#include "find_levels.hpp"

template<class T>
FindLevelsResult find_levels(
        // Set of edge disjoint paths in G'.
        // (actually just a array of edges in G' which are on some path)
        const std::vector<Edge> &blocked_edges,
        // Disks representing the vertices of G
        const std::vector<Disk<T>> &disks,
        // Left and right boundary of the available space
        const T &left_border_x,
        const T &right_border_x,
        const Config<T> &config
) {
    auto levels = std::unordered_map<TransformedVertex, int, TransformedVertexHash>();
    std::vector<bool> used_disks(disks.size(), false);

    const auto left_border = Border<T>{left_border_x, true};
    const auto right_border = Border<T>{right_border_x, false};

    // Preprocessing: for every vertex on any of the paths, mark previous and next vertex on the path.
    std::unordered_map<TransformedVertex, TransformedVertex, TransformedVertexHash> prev;
    std::unordered_map<TransformedVertex, TransformedVertex, TransformedVertexHash> next;

    for (const auto e: blocked_edges) {
        // Add previous vertex
        prev[e.to] = e.from;
        // Add next vertex
        next[e.from] = e.to;
    }

    // First layer - layer 0 - level of source is 0
    levels[source] = 0;

    // Construct data structure from disks and sink.
    std::vector<GeometryObject<T>> objects(disks.begin(), disks.end());
    objects.push_back(right_border);

    DataStructure<T> * ds = config.data_structure_constructor();
    ds->rebuild(objects);

    // Find layer 1 - query datastructure for disks intersecting with the left border
    std::vector<Disk<T>> u_neighbors;
    bool found_sink = false;

    while (true) {
        // Find disk intersecting with the left border
        auto n = ds->intersecting(left_border);

        if (n.has_value()) {
            const GeometryObject<T> o = n.value();
            // Remove object from data structure
            ds->delete_object(o);

            // If the object is a disk, add it to the list of neighbors
            if (is_disk(o)) {
                u_neighbors.push_back(std::get<Disk<T>>(o));
            } else {
                // If the object is a border, we found the sink.
                found_sink = true;
                break;
            }
        } else {
            // If there are no disks intersecting with the left border, we are done.
            break;
        }
    }

    // If we found the sink, we are done.
    if (found_sink) {
        levels[sink] = 1;
        return {levels, true, 1, prev, next};
    }

    // Convert neighboring geometry objects to inbound vertices (there are only inbound vertices on layer 1)
    std::vector<TransformedVertex> u_neighbors_vertices(u_neighbors.size());
    std::transform(u_neighbors.begin(), u_neighbors.end(), u_neighbors_vertices.begin(), [](const auto &d) {
        return disk_to_transformed_vertex(d, true);
    });

    // Filter out vertices that have previous vertex s.
    u_neighbors_vertices.erase(std::remove_if(
                                       u_neighbors_vertices.begin(), u_neighbors_vertices.end(),
                                       [&prev](const auto &v) {
                                           // Check if v is in path and has previous vertex s
                                           return prev.find(v) != prev.end() && prev[v] == source;
                                       }),
                               u_neighbors_vertices.end());

    // Add vertices to the list of vertices on layer 1
    for (const auto &v: u_neighbors_vertices) {
        levels[v] = 1;
        // Mark disk as used
        used_disks[v.disk_index] = true;
    }

    // Reconstruct data structure from remaining disks and sink.
    objects = {};
    for (unsigned int i = 0; i < disks.size(); i++) {
        if (!used_disks[i]) {
            objects.push_back(disks[i]);
        }
    }
    objects.push_back(right_border);
    ds->rebuild(objects);

    // Last layer, L[i - 1]
    std::vector<TransformedVertex> last_layer_vertices = u_neighbors_vertices;
    std::vector<TransformedVertex> current_layer_vertices;
    int i = 2;

    // While last layer is not empty and does not contain the sink
    while (!last_layer_vertices.empty() && !found_sink) {
        // Compute L[i] - new layer
        current_layer_vertices = {};

        if (i % 2 == 0) {
            // If i is even, we iterate over the inbound vertices of the last layer
            for (auto v: last_layer_vertices) {
                if (!v.inbound) {
                    continue;
                }

                // - if inbound vertex lies on some path, graph L contains reverse edge prev[v_inbound] -> v_inbound
                // - if inbound vertex does not lie on a path, we can add v_outbound to L[i]
                if (prev.find(v) != prev.end()) {
                    auto u = prev[v];
                    if (u == source) {
                        // We do not need to do anything, since we already added source to L[0]
                    } else {
                        // We add u to current layer
                        levels[u] = i;
                        current_layer_vertices.push_back(u);
                    }
                } else {
                    // Add outbound vertex to the graph and to the current layer
                    levels[{v.disk_index, false}] = i;
                    current_layer_vertices.push_back({v.disk_index, false});
                }
            }
        } else {
            // If i is odd, we iterate over the outbound vertices of the last layer
            for (auto v: last_layer_vertices) {
                if (v.inbound) {
                    continue;
                }

                std::vector<Disk<T>> neighbors;

                // Query data structure for objects intersecting with the disk
                while (true) {
                    auto n = ds->intersecting(disks[v.disk_index]);

                    if (n.has_value()) {
                        const GeometryObject<T> o = n.value();
                        // Remove object from data structure
                        ds->delete_object(o);

                        // If the object is a disk, add it to the list of neighbors
                        if (is_disk(o)) {
                            neighbors.push_back(std::get<Disk<T>>(o));
                        } else {
                            // If the object is a border, we found the sink.
                            found_sink = true;
                            levels[sink] = i;
                            break;
                        }
                    } else {
                        // If there are no disks intersecting with the disk, we are done.
                        break;
                    }
                }

                // If we found the sink - clear what we did in current layer and break
                if (found_sink) {
                    for (const auto &v: current_layer_vertices) {
                        levels.erase(v);
                    }
                    current_layer_vertices.clear();
                    break;
                }

                // If there are no neighbors, go to the next vertex
                if (neighbors.empty()) {
                    continue;
                }

                // Convert neighboring geometry objects to inbound vertices
                std::vector<TransformedVertex> neighbors_vertices(neighbors.size());
                std::transform(neighbors.begin(), neighbors.end(), neighbors_vertices.begin(), [](const auto &d) {
                    return disk_to_transformed_vertex(d, true);
                });

                // If v_outbound lies on some path, graph L contains reverse edge v_outbound -> prev[v_outbound] = u_inbound
                // In this case, we need to ignore v_outbound -> next[v_outbound] edge.
                bool has_next = next.find(v) != next.end();
                for (const auto &u: neighbors_vertices) {
                    if (has_next && next[v] == u) {
                        // Ignore edge v_outbound -> next[v_outbound]
                        // Reverse edge is v_outbound -> v_inbound. We already added v_inbound to the layer L[i - 2],
                        // (v is in layer L[i - 2]) so we do not need to do anything.
                        continue;
                    }

                    // Add u to current layer
                    levels[u] = i;
                    current_layer_vertices.push_back(u);
                }
            }
        }

        i++;
        last_layer_vertices = current_layer_vertices;
    }

    int distance;
    if (found_sink) {
        distance = levels[sink];
    } else {
        distance = -1;
    }

    return {levels, found_sink, distance, prev, next};
}

// Force compiler to instantiate the template for the types we need
template FindLevelsResult find_levels<int>(const std::vector<Edge> &blocked_edges, const std::vector<Disk<int>> &disks,
        const int &left_border_x, const int &right_border_x, const Config<int> &config);

template FindLevelsResult find_levels<double>(const std::vector<Edge> &blocked_edges, const std::vector<Disk<double>> &disks,
        const double &left_border_x, const double &right_border_x, const Config<double> &config);
