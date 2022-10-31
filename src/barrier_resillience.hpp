#ifndef BARRIER_RESILIENCE_BARRIER_RESILIENCE_HPP
#define BARRIER_RESILIENCE_BARRIER_RESILIENCE_HPP

#include <vector>
#include "geometry_objects.hpp"
#include "graph.hpp"

template<class T>
std::vector<GeometryObject<T>> prepare_objects_vector(const std::vector<Disk<T>>& disks, const T left_border_x, const T right_border_x) {
    // Create new vector of disks with left and right borders.
    auto objects = std::vector<GeometryObject<T>>{};

    // Left border.
    objects.push_back(Border<T>{left_border_x, true});
    
    // Disks.
    for (const auto& disk : disks) {
        objects.push_back(disk);
    }

    // Right border.
    objects.push_back(Border<T>{right_border_x, false});

    return objects;
}

template<class T>
void barrier_resilience(const std::vector<Disk<T>>& disks, const T left_border_x, const T right_border_x) {
    // Get vector of disks with left and right borders.
    auto objects = prepare_objects_vector(disks, left_border_x, right_border_x);

    // Start and end indices - left and right borders.
    // (see how prepare_objects_vector works).
    int left_border_index = 0;
    int right_border_index = objects.size() - 1;

    // Create graph of objects.
    auto graph = objects_to_graph(objects);

    // TODO Expand each node representing a disk.

    // TODO Compute max flow in graph from start to end.

}


#endif // BARRIER_RESILIENCE_BARRIER_RESILIENCE_HPP