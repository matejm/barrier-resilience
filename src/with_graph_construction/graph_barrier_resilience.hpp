#ifndef BARRIER_RESILIENCE_GRAPH_BARRIER_RESILIENCE_HPP
#define BARRIER_RESILIENCE_GRAPH_BARRIER_RESILIENCE_HPP

#include <vector>
#include <iostream>
#include "utils/geometry_objects.hpp"
#include "graph.hpp"
#include "ford_fulkerson.hpp"
#include "even_tarjan.hpp"

enum class Algorithm {
    FordFulkerson,
    EvenTarjan
};

// Returns just minimal number of disks that need to be removed.
template<class T>
int graph_barrier_resilience_number_of_disks(const std::vector<Disk<T>> &disks,
                                             const T left_border_x,
                                             const T right_border_x,
                                             Algorithm algorithm = Algorithm::FordFulkerson) {
    // Create graph of objects.
    Graph graph = generate_expanded_graph(disks, left_border_x, right_border_x);

    // Start and end indices - left and right borders.
    int left_border_index = graph_start_index(graph);
    int right_border_index = graph_end_index(graph);

    // Return minimal number of circles that need to be removed.
    switch (algorithm) {
        case Algorithm::FordFulkerson:
            return ford_fulkerson_max_flow(graph, left_border_index, right_border_index);
        case Algorithm::EvenTarjan:
            return even_tarjan_max_flow(graph, left_border_index, right_border_index);
    }
}

// Returns indices of specific disks that need to be removed.
template<class T>
std::vector<int> graph_barrier_resilience_disks(const std::vector<Disk<T>> &disks,
                                                const T left_border_x,
                                                const T right_border_x,
                                                Algorithm algorithm = Algorithm::FordFulkerson) {
    // Create graph of objects.
    auto graph = generate_expanded_graph(disks, left_border_x, right_border_x);

    // Start and end indices - left and right borders.
    int left_border_index = graph_start_index(graph);
    int right_border_index = graph_end_index(graph);

    // Return minimal number of circles that need to be removed.
    std::vector<std::pair<int, int>> edges;

    switch (algorithm) {
        case Algorithm::FordFulkerson:
            edges = ford_fulkerson_min_cut(graph, left_border_index, right_border_index);
            break;
        case Algorithm::EvenTarjan:
            edges = even_tarjan_min_cut(graph, left_border_index, right_border_index);
            break;
    }

    // Find indices of disks that need to be removed.
    // We need to remove disks which have inbound nodes as part of min cut.
    std::vector<int> disks_to_remove;

    for (const auto &edge: edges) {
        auto [_, v] = edge;
        disks_to_remove.push_back(graph_index_to_disk_index(graph, v));
    }

    return disks_to_remove;
}


#endif // BARRIER_RESILIENCE_GRAPH_BARRIER_RESILIENCE_HPP