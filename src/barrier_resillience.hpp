#ifndef BARRIER_RESILIENCE_BARRIER_RESILIENCE_HPP
#define BARRIER_RESILIENCE_BARRIER_RESILIENCE_HPP

#include <vector>
#include <iostream>
#include "geometry_objects.hpp"
#include "graph.hpp"
#include "ford_fulkerson.hpp"

template<class T>
int barrier_resilience(const std::vector<Disk<T>>& disks, const T left_border_x, const T right_border_x) {
    // Create graph of objects.
    auto graph = generate_expanded_graph(disks, left_border_x, right_border_x);

    // Start and end indices - left and right borders.
    int left_border_index = graph_start_index(graph);
    int right_border_index = graph_end_index(graph);

    // Return minimal number of circles that need to be removed.
    return ford_fulkerson_max_flow(graph, left_border_index, right_border_index);
}


#endif // BARRIER_RESILIENCE_BARRIER_RESILIENCE_HPP