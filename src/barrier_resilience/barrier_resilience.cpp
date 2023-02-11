#include "barrier_resilience.hpp"

std::vector<Edge> direct_sum(const std::vector<Edge> &edges, const std::vector<Path> &paths) {
    std::unordered_map<Edge, int, EdgeHash> edge_count = {};

    for (const auto &edge: edges) {
        edge_count[edge] = true;
    }
    for (const auto &path: paths) {
        for (const auto &edge: path) {
            edge_count[edge] = true;
        }
    }

    // Include only edges that appear exactly once.
    // If edge appears twice, discard it.
    // An edge should not appear more than twice.
    std::vector<Edge> result = {};
    for (const auto &p: edge_count) {
        if (p.second == 1) {
            result.push_back(p.first);
        }
    }

    return result;
}

template<class T>
int barrier_resilience_number_of_disks(std::vector<Disk<T>> &disks,
                                       const T &left_border_x,
                                       const T &right_border_x,
                                       const Config<T> &config) {
    // Set index to each disk (so we can track them in the data structure)
    add_index<T>(disks);

    // We won't have a family of paths, but just a collection of all edges.
    // (that way, we can compute direct sum of all edges)
    std::vector<Edge> edges = {};

    // Number of disjoint paths = number of disks.
    // (measuring max s-t flow in a graph)
    int path_count = 0;

    while (true) {
        // Find blocking family of paths.
        auto blocking_family = find_blocking_family<T>(edges, disks, left_border_x, right_border_x, config);

        if (blocking_family.empty()) {
            // No more paths to find.
            break;
        }

        path_count += blocking_family.size();

        // Perform direct sum of all edges in the family.
        edges = direct_sum(edges, blocking_family);
    }

    return path_count;
}

// Force compiler to instantiate template for int and double
template int barrier_resilience_number_of_disks<int>(std::vector<Disk<int>> &disks,
                                                     const int &left_border_x,
                                                     const int &right_border_x,
                                                     const Config<int> &config);

template int barrier_resilience_number_of_disks<double>(std::vector<Disk<double>> &disks,
                                                        const double &left_border_x,
                                                        const double &right_border_x,
                                                        const Config<double> &config);