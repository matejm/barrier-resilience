#ifndef BARRIER_RESILIENCE_BLOCKING_FAMILY_HPP
#define BARRIER_RESILIENCE_BLOCKING_FAMILY_HPP

#include <vector>
#include <ranges>
#include <optional>
#include "utils/geometry_objects.hpp"
#include "utils/transformed_graph.hpp"
#include "data_structure/data_structure.hpp"
#include "find_levels.hpp"

template<class T, template<typename = T> class DS>
std::optional<std::vector<TransformedVertex>> dfs_explore(
        // Used to query intersecting disks.
        std::vector<DS<T>> &ds,
        const std::vector<Disk<T>> &disks,
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

    // Ignore if already explored
    if (explored.find(v) != explored.end()) {
        return std::nullopt;
    }

    // Add vertex to path
    current_path.push_back(v);
    std::optional<std::vector<TransformedVertex>> path = std::nullopt;

    if (current_level % 2 == 1) {
        // Odd level (v is inbound vertex)
        if (prev.find(v) == prev.end()) {
            // Inbound vertex v is not on a path -> continue DFS at outbound vertex of same disk

            path = dfs_explore(ds, disks, explored, prev, next,
                               TransformedVertex{v.disk_index, false}, current_level + 1,
                               sink_level, left_border, has_edge_to_sink, current_path);
        } else {
            // Inbound vertex v is on a path -> go back to previous vertex (if not explored yet)
            auto p = prev[v];
            if (explored.find(p) == explored.end()) {
                current_path.push_back(v);
                path = dfs_explore(ds, disks, explored, prev, next,
                                   p, current_level + 1,
                                   sink_level, left_border, has_edge_to_sink, current_path);
            }
        }
    } else {
        // Even level (v is outbound vertex)

        // First check if we are at last vertex before sink
        if (current_level == sink_level - 1) {
            // Check if we can get directly to sink
            // - disk of v should be intersected by right border
            // - edge (v, sink) should not be blocked - next[v] should not be sink.
            if (has_edge_to_sink(disks[v.disk_index]) && next[v] != sink) {
                // We can get directly to sink - found path
                current_path.push_back(sink);
                // Can return here, no need to clean-up current_path, DFS will return anyway as we found path.
                return current_path;
            }
        } else {
            // We are exploring outbound vertex v.
            // This case also includes source vertex (which is always outbound).

            // First check if v is on a path
            if (prev.find(v) != prev.end()) {
                // Then next vertex is inbound vertex of same disk (go back)
                auto v_in = TransformedVertex{v.disk_index, true};
                if (explored.find(v_in) == explored.end()) {
                    // Remove disk from data structure[current_level + 1]
                    ds[current_level + 1].delete_object(disks[v.disk_index]);

                    current_path.push_back(v);
                    path = dfs_explore(ds, disks, explored, prev, next,
                                       v_in, current_level + 1,
                                       sink_level, left_border, has_edge_to_sink, current_path);
                }
            }

            // If nothing found, then explore all other connections from v
            if (!path.has_value()) {
                // Explore all other connections from v (that are not on paths)
                // Query data structure for intersecting disks

                bool is_source = v == source;

                while (!path.has_value()) {
                    // Check if disk of v is intersected by any disk in data structure[current_level + 1]
                    // If v is source, then compute intersection with left border
                    auto r = is_source
                             ? ds[current_level + 1].intersecting(left_border)
                             : ds[current_level + 1].intersecting(disks[v.disk_index]);

                    if (!r.has_value()) {
                        // No intersecting disk found
                        break;
                    }
                    // Cannot be other than disk (cannot be border)
                    std::variant<Disk<T>, Border<T>> disk_or_border = r.value();
                    auto disk = std::get<Disk<T>>(disk_or_border);

                    // Inbound vertex of disk, we have an edge v->u in residual graph.
                    // This edge is not in any path, see article for details.
                    auto u = TransformedVertex{disk.index, true};

                    path = dfs_explore(ds, disks, explored, prev, next,
                                       u, current_level + 1,
                                       sink_level, left_border, has_edge_to_sink, current_path);
                }
            }
        }
    }

    // Remove vertex from path
    current_path.pop_back();

    // Mark vertex as explored (if this is not source)
    explored[v] = true;

    return path;
}

Path list_of_vertices_to_path(const std::vector<TransformedVertex> &vertices) {
    Path path;
    for (int i = 1; i < vertices.size(); i++) {
        path.push_back(Edge(vertices[i - 1], vertices[i]));
    }
    return path;
}

// DS is a data structure implementation
template<class T, template<typename> class DS>
std::vector<Path> find_blocking_family(
        // Set of edge disjoint paths in G'
        const std::vector<Path> &paths,
        // Disks representing the vertices of G
        std::vector<Disk<T>> &disks,
        // Left and right boundary of the available space
        const int left_border_x,
        const int right_border_x) {

    // First, compute level for each vertex
    auto r = find_levels<T, DS>(paths, disks, left_border_x, right_border_x);

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
    auto data_structures = std::vector<DS<T>>(r.distance + 1, DS<T>());

    // A little change from the article:
    // We do not need to build for last level, because it contains only sink (therefore < instead of <=).
    for (unsigned int i = 1; i < r.distance; i += 2) {
        // For each odd i we build a data structure ds for inbound vertices of level i
        std::vector<GeometryObject<T>> inbound_vertices;
        for (const auto &v: vertices_by_level[i]) {
            if (v.inbound) {
                inbound_vertices.push_back(disks[v.disk_index]);
            }
        }
        data_structures[i].rebuild(inbound_vertices);
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

    int j = 1;
    while (true) {
        // We perform DFS traversal from the source.
        // When we get to sink, we have found a path. We add it to the new path family.
        auto new_path = dfs_explore<T, DS>(
                data_structures,
                disks,
                explored,
                r.prev,
                r.next,
                source,  // Start DFS at source
                0,  // Level of source is 0
                r.distance,  // Level of sink is r.distance
                left_border,
                [&](const Disk<T> &disk) { return intersects(disk, right_border); },
                empty_path  // Start with empty path
        );
        empty_path.clear(); // Clear path for next iteration

        if (!new_path.has_value()) {
            // No more paths found
            break;
        }

        new_paths.push_back(list_of_vertices_to_path(new_path.value()));
    }

    // Found a blocking family
    std::cout << "Found a blocking family of size " << new_paths.size() << std::endl;
    return new_paths;
}

#endif //BARRIER_RESILIENCE_BLOCKING_FAMILY_HPP