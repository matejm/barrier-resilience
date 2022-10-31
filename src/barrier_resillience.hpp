#ifndef BARREIR_RESILLIENCE_BARRIER_RESILLIENCE_HPP
#define BARREIR_RESILLIENCE_BARRIER_RESILLIENCE_HPP

#include <vector>
#include "geometry_objects.hpp"
#include "graph.hpp"

template<class T>
void barrier_resillience(const std::vector<Circle<T>>& circles, const Point<T>& start, const Point<T>& end) {
    int left_circle = leftmost_circle(circles);
    int right_circle = rightmost_circle(circles);

    auto graph = circles_to_graph(circles);

    // Compute max flow in graph from leftmost circle to rightmost circle.

}


#endif // BARREIR_RESILLIENCE_BARRIER_RESILLIENCE_HPP