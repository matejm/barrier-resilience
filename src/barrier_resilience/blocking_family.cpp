#include "blocking_family.hpp"


template<class T>
std::optional<std::vector<TransformedVertex>> dfs_explore(
        // Used to query intersecting disks.
        std::vector<DataStructure<T> *> &ds,
        const std::vector<Disk<T>> &disks,
        std::unordered_map<TransformedVertex, int, TransformedVertexHash> levels,
        // Visited vertices. Source and sink are never marked as visited; we can visit them multiple times.
        std::unordered_map<TransformedVertex, bool, TransformedVertexHash> &explored,
        // Previous vertex in the path (if vertex is on any of paths in given path family which defines residual graph)
        std::unordered_map<TransformedVertex, TransformedVertex, TransformedVertexHash> prev,
        std::unordered_map<TransformedVertex, TransformedVertex, TransformedVertexHash> next,
        TransformedVertex v,
        int current_level,
        int sink_level,
        // Left border (source)
        const Border<T> &left_border,
        // Function which tells us if we can get to sink from given disk without any additional hops
        const std::function<bool(Disk<T>)> &has_edge_to_sink,
        std::vector<TransformedVertex> &current_path) {
    // Add vertex to path
    current_path.push_back(v);
    std::optional<std::vector<TransformedVertex>> path;

    if (current_level % 2 == 1) {
        // Odd level (v is inbound vertex)
        assert(v.inbound);

        if (!prev.contains(v)) {
            // Inbound vertex v is not on a path -> continue DFS at outbound vertex of same disk
            auto u = TransformedVertex{v.disk_index, false};
            if (!explored[u]) {
                explored[u] = true;
                path = dfs_explore(ds, disks, levels, explored, prev, next,
                                   u, current_level + 1,
                                   sink_level, left_border, has_edge_to_sink, current_path);
            }
        } else {
            // Inbound vertex v is on a path -> go back to previous vertex (if not explored yet)
            auto p = prev[v];
            if (!explored[p]) {
                // Minor correction of the article: don't always go back in the path. If level is not current_level + 1,
                // then we are going to vertex which has a level <= current_level. This means that there is a better
                // path in a tree to this vertex. We don't want to go back to this vertex from here, because then this
                // won't be a tree anymore.
                if (levels[p] == current_level + 1) {
                    explored[p] = true;
                    path = dfs_explore(ds, disks, levels, explored, prev, next,
                                       p, current_level + 1,
                                       sink_level, left_border, has_edge_to_sink, current_path);
                }
            }
        }
    } else {
        // Even level (v is outbound vertex)
        assert(!v.inbound);

        bool is_source = v == source;

        // First check if we are at last vertex before sink
        // Shortest possible path source -> sink is not considered a path.
        if (!is_source && current_level == sink_level - 1) {
            // Check if we can get directly to sink
            // - disk of v should be intersected by right border
            // - edge (v, sink) should not be blocked - next[v] should not be sink.
            if (has_edge_to_sink(disks[v.disk_index]) && next[v] != sink) {
                // We can get directly to sink - found path
                // Copy current_path to path
                auto p = std::vector<TransformedVertex>(current_path);
                p.push_back(sink);

                path = {p};
            }
        } else {
            // We are exploring outbound vertex v.
            // This case also includes source vertex (which is always outbound).

            // First check if v is on a path
            if (prev.contains(v)) {
                // Then next vertex is inbound vertex of same disk (go back)
                auto v_in = TransformedVertex{v.disk_index, true};
                if (!explored[v_in]) {
                    // Similar as above, do not continue in the path if level is not current_level + 1
                    if (levels[v_in] == current_level + 1) {
                        // Remove disk from data structure[current_level + 1]
                        ds[current_level + 1]->delete_object(disks[v.disk_index]);

                        explored[v_in] = true;
                        path = dfs_explore(ds, disks, levels, explored, prev, next,
                                           v_in, current_level + 1,
                                           sink_level, left_border, has_edge_to_sink, current_path);
                    }
                }
            }

            // Explore all other connections from v (that are not on paths)
            // Query data structure for intersecting disks

            // If nothing found, then explore all other connections from v (if we found path, we won't enter this loop)
            while (!path.has_value()) {
                // Check if disk of v is intersected by any disk in data structure[current_level + 1]
                // If v is source, then compute intersection with left border
                auto r = is_source
                         ? ds[current_level + 1]->intersecting(left_border)
                         : ds[current_level + 1]->intersecting(disks[v.disk_index]);

                if (!r.has_value()) {
                    // No intersecting disk found, done with this vertex
                    break;
                }

                // Cannot be other than disk (cannot be border)
                std::variant<Disk<T>, Border<T>> disk_or_border = r.value();
                auto disk = std::get<Disk<T>>(disk_or_border);

                // Inbound vertex of disk, we have an edge v->u in residual graph.
                // This edge is not in any path, see article for details.
                auto u = TransformedVertex{disk.get_index(), true};

                // Remove disk from data structure
                ds[current_level + 1]->delete_object(disk);

                if (explored[u]) {
                    // Vertex u is already explored, continue with next disk
                    continue;
                }

                // Mark explored
                explored[u] = true;

                path = dfs_explore(ds, disks, levels, explored, prev, next,
                                   u, current_level + 1,
                                   sink_level, left_border, has_edge_to_sink, current_path);

            }
        }
    }

    // Remove vertex from path
    current_path.pop_back();

    return path;
}


template<class T>
std::vector<Path> find_blocking_family(
        // Set of edge disjoint paths in G' (multiple paths specified as list of edges)
        const std::vector<Edge> &blocked_edges,
        // Disks representing the vertices of G
        const std::vector<Disk<T>> &disks,
        // Left and right boundary of the available space
        const T left_border_x,
        const T right_border_x,
        const Config<T> &config) {

    if (disks.empty()) {
        // No disks -> nothing to find
        // (path source -> sink without any disks does not count as a path and happens only in case when left and right
        // border are the same or left border is to the right of right border)
        return {};
    }

    // First, compute level for each vertex
    auto r = find_levels<T>(blocked_edges, disks, left_border_x, right_border_x, config);

    if (!r.reachable) {
        // If sink is not reachable, then there is no blocking family (blocking family exits -> it is an empty set)
        return {};
    }

    // Group vertices by level
    std::vector<std::vector<TransformedVertex>> vertices_by_level(r.distance + 1);
    for (const auto &p: r.levels) {
        vertices_by_level[p.second].push_back(p.first);
    }

    // Construct data structure for each odd level
    // data_structures[i] (for odd i) will contain vertices v_in
    auto data_structures = std::vector<DataStructure<T> *>(r.distance + 1);
    for (unsigned int i = 0; i < data_structures.size(); i++) {
        data_structures[i] = config.data_structure_constructor();
    }

    // A little change from the article:
    // We do not need to build for last level, because it contains only sink (therefore < instead of <=).
    for (int i = 1; i < r.distance; i += 2) {
        // For each odd i we build a data structure ds for inbound vertices of level i
        std::vector<GeometryObject<T>> inbound_vertices;
        for (const auto &v: vertices_by_level[i]) {
            if (v.inbound) {
                inbound_vertices.push_back(disks[v.disk_index]);
            }
        }
        data_structures[i]->rebuild(inbound_vertices);
    }

    // Find blocking path in layered residual graph.
    // DFS traversal of the graph, starting from the source.

    // We start with new path family
    std::vector<Path> new_paths;
    // Which vertices are already explored?
    std::unordered_map<TransformedVertex, bool, TransformedVertexHash> explored;

    Border<T> right_border = {right_border_x, false};
    Border<T> left_border = {left_border_x, true};

    std::vector<TransformedVertex> empty_path;

    while (true) {
        // We perform DFS traversal from the source.
        // When we get to sink, we have found a path. We add it to the new path family.
        auto new_path = dfs_explore<T>(
                data_structures,
                disks,
                r.levels,
                explored,
                r.prev,
                r.next,
                source,  // Start DFS at source
                0,  // Level of source is 0
                r.distance,  // Level of sink is r.distance
                left_border,
                [&](const Disk<T> &disk) { return intersects<T>(disk, right_border); },
                empty_path  // Start with empty path
        );
        empty_path.clear(); // Clear path for next iteration

        if (!new_path.has_value()) {
            // No more paths found
            break;
        }

        new_paths.push_back(list_of_vertices_to_path(new_path.value()));
    }

    // If sink is reachable, then there is always a blocking family.
    assert(r.reachable && !new_paths.empty());

    // Found a blocking family
    return new_paths;
}

static Path list_of_vertices_to_path(const std::vector<TransformedVertex> &vertices) {
    Path path;
    for (unsigned int i = 1; i < vertices.size(); i++) {
        path.push_back(Edge(vertices[i - 1], vertices[i]));
    }
    return path;
}


// Force compiler to generate code for these types
template std::vector<Path> find_blocking_family<int>(
        const std::vector<Edge> &blocked_edges,
        const std::vector<Disk<int>> &disks,
        const int left_border_x,
        const int right_border_x,
        const Config<int> &config);

template std::vector<Path> find_blocking_family<double>(
        const std::vector<Edge> &blocked_edges,
        const std::vector<Disk<double>> &disks,
        const double left_border_x,
        const double right_border_x,
        const Config<double> &config);

