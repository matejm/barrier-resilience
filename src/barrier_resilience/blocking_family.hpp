#ifndef BARRIER_RESILIENCE_BLOCKING_FAMILY_HPP
#define BARRIER_RESILIENCE_BLOCKING_FAMILY_HPP

#include <vector>
#include <ranges>
#include <optional>
#include "utils/geometry_objects.hpp"
#include "utils/transformed_graph.hpp"
#include "data_structure/data_structure.hpp"
#include "find_levels.hpp"
#include "config.hpp"


template<class T>
std::vector<Path> find_blocking_family(
        // Set of edge disjoint paths in G' (multiple paths specified as list of edges)
        const std::vector<Edge> &blocked_edges,
        // Disks representing the vertices of G
        const std::vector<Disk<T>> &disks,
        // Left and right boundary of the available space
        const T left_border_x,
        const T right_border_x,
        const Config<T> &config);

#endif //BARRIER_RESILIENCE_BLOCKING_FAMILY_HPP
