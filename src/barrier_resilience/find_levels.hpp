#ifndef BARRIER_RESILIENCE_FIND_LEVELS_HPP
#define BARRIER_RESILIENCE_FIND_LEVELS_HPP

#include <vector>
#include <unordered_map>
#include "utils/geometry_objects.hpp"
#include "utils/transformed_graph.hpp"
#include "data_structure/data_structure.hpp"
#include "config.hpp"

struct FindLevelsResult {
    // TODO: unordered map can be swapped for a std::vector<pair<int, int>> (index of a vector is a border index, then two values for inboud and outbound edges)
    std::unordered_map<TransformedVertex, int, TransformedVertexHash> levels;
    // True if there is a path from left border to right border.
    bool reachable;
    // Total distance to the sink, if reachable.
    int distance;
    // Map of previous vertices on the path from source to sink.
    // Warning: prev[sink] might be incorrect (we can get to sink from multiple vertices).
    std::unordered_map<TransformedVertex, TransformedVertex, TransformedVertexHash> prev;
    // Map of next vertices on the path from source to sink.
    // Warning: next[source] might be incorrect (there can be multiple paths from source to sink).
    std::unordered_map<TransformedVertex, TransformedVertex, TransformedVertexHash> next;
};

// Find BFS distance from source for each vertex v of a graph G' (lambda(v) in the article).
// Works by performing BFS from source in the residual graph R(G', paths) without explicit construction of the edge set
// of R.

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
);

#endif //BARRIER_RESILIENCE_FIND_LEVELS_HPP
