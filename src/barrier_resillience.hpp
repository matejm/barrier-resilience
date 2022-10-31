#ifndef BARRIER_RESILIENCE_BARRIER_RESILIENCE_HPP
#define BARRIER_RESILIENCE_BARRIER_RESILIENCE_HPP

#include <vector>
#include <iostream>
#include "geometry_objects.hpp"
#include "graph.hpp"
#include "ford_fulkerson.hpp"

// Returns just minimal number of disks that need to be removed.
template<class T>
int barrier_resilience_number_of_disks(const std::vector<Disk<T>>& disks, const T left_border_x, const T right_border_x) {
    // Create graph of objects.
    auto graph = generate_expanded_graph(disks, left_border_x, right_border_x);

    // Start and end indices - left and right borders.
    int left_border_index = graph_start_index(graph);
    int right_border_index = graph_end_index(graph);

    // Return minimal number of circles that need to be removed.
    return ford_fulkerson_max_flow(graph, left_border_index, right_border_index);
}

// Returns indices of specific disks that need to be removed.
template<class T>
std::vector<int> barrier_resilience_disks(const std::vector<Disk<T>>& disks, const T left_border_x, const T right_border_x) {
    // Create graph of objects.
    auto graph = generate_expanded_graph(disks, left_border_x, right_border_x);

    // Start and end indices - left and right borders.
    int left_border_index = graph_start_index(graph);
    int right_border_index = graph_end_index(graph);

    // Return minimal number of circles that need to be removed.
    std::vector<std::pair<int, int>> edges = ford_fulkerson_min_cut(graph, left_border_index, right_border_index);

    // Find indices of disks that need to be removed.
    // We need to remove disks which have inbound nodes as part of min cut.
    std::vector<int> disks_to_remove;

    for (const auto& edge : edges) {
        auto [_, v] = edge;
        disks_to_remove.push_back(graph_index_to_disk_index(graph, v));
    }

    return disks_to_remove;
}


#endif // BARRIER_RESILIENCE_BARRIER_RESILIENCE_HPP