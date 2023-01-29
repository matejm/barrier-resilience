#ifndef BARRIER_RESILIENCE_FIND_LEVELS_HPP
#define BARRIER_RESILIENCE_FIND_LEVELS_HPP

#include <vector>
#include <unordered_map>
#include "utils/geometry_objects.hpp"
#include "utils/transformed_graph.hpp"
#include "data_structure/data_structure.hpp"

struct FindLevelsResult {
    // TODO: unordered map can be swapped for a std::vector<pair<int, int>> (index of a vector is a border index, then two values for inboud and outbound edges)
    std::unordered_map<TransformedVertex, int, TransformedVertexHash> levels;
    // True if there is a path from left border to right border.
    bool reachable;
};

// Find BFS distance from source for each vertex v of a graph G' (lambda(v) in the article).
// Works by performing BFS from source in the residual graph R(G', paths) without explicit construction of the edge set
// of R.

template<class T>
FindLevelsResult find_levels(
        // Set of edge disjoint paths in G'.
        const std::vector<Path> &paths,
        // Disks representing the vertices of G
        std::vector<Disk<T>> &disks,
        // Left and right boundary of the available space
        const int left_border_x,
        const int right_border_x,
        // Data structure implementation
        DataStructure<T> &ds
) {
    // Set index to each disk (so we can track them in the data structure)
    for (int i = 0; i < disks.size(); i++) {
        disks[i].set_index(i);
    }

    auto levels = std::unordered_map<TransformedVertex, int, TransformedVertexHash>();
    std::vector<bool> used_disks(disks.size(), false);

    const auto left_border = Border<T>{left_border_x, true};
    const auto right_border = Border<T>{right_border_x, false};

    // Preprocessing: for every vertex on any of the paths, mark previous and next vertex on the path.
    std::unordered_map<TransformedVertex, TransformedVertex, TransformedVertexHash> prev;
    std::unordered_map<TransformedVertex, TransformedVertex, TransformedVertexHash> next;

    for (const auto &path: paths) {
        for (unsigned int i = 0; i < path.size(); i++) {
            const auto& e = path[i];
            // Add previous vertex
            prev[e.to] = e.from;
            // Add next vertex
            next[e.from] = e.to;
        }
    }

    // First layer - layer 0 - level of source is 0
    levels[source] = 0;

    // Construct data structure from disks and sink.
    std::vector<GeometryObject<T>> objects(disks.begin(), disks.end());
    objects.push_back(right_border);

    ds.rebuild(objects);

    // Find layer 1 - query datastructure for disks intersecting with the left border
    std::vector<Disk<T>> u_neighbors;
    bool found_sink = false;

    while (true) {
        // Find disk intersecting with the left border
        auto n = ds.intersecting(left_border);

        if (n.has_value()) {
            const GeometryObject<T> o = n.value();
            // Remove object from data structure
            ds.delete_object(o);

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
        return {levels, true};
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
    ds.rebuild(objects);

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
                    auto n = ds.intersecting(disks[v.disk_index]);

                    if (n.has_value()) {
                        const GeometryObject<T> o = n.value();
                        // Remove object from data structure
                        ds.delete_object(o);

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

                // If we found the sink, we are done.
                // If there are no neighbors, we are done.
                if (found_sink || neighbors.empty()) {
                    break;
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

            // If we found the sink - clear what we did in current layer and break
            if (found_sink) {
                for (const auto &v: current_layer_vertices) {
                    levels.erase(v);
                }
                current_layer_vertices.clear();
                break;
            }
        }

        i++;
        last_layer_vertices = current_layer_vertices;
    }

    return {levels, found_sink};
}

#endif //BARRIER_RESILIENCE_FIND_LEVELS_HPP
