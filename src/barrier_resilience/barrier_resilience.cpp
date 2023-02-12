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

struct BlockingPathsResult {
    int path_count;
    std::vector<Edge> edges;
};

// Find blocking family of paths.
// (returned as a collection of edges)
template<class T>
BlockingPathsResult get_blocking_paths(std::vector<Disk<T>> disks,
                                       const T &left_border_x, const T &right_border_x,
                                       const Config<T> &config) {
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

    return BlockingPathsResult{path_count, edges};
}

template<class T>
int barrier_resilience_number_of_disks(std::vector<Disk<T>> &disks,
                                       const T &left_border_x,
                                       const T &right_border_x,
                                       const Config<T> &config) {
    // Set index to each disk (so we can track them in the data structure)
    add_index_to_disks(disks);

    auto r = get_blocking_paths(disks, left_border_x, right_border_x, config);
    return r.path_count;
}

template<class T>
std::vector<int> barrier_resilience_disks(std::vector<Disk<T>> &disks,
                                          const T &left_border_x,
                                          const T &right_border_x,
                                          const Config<T> &config) {
    // Set index to each disk (so we can track them in the data structure)
    add_index_to_disks(disks);

    auto r = get_blocking_paths(disks, left_border_x, right_border_x, config);

    // Find disks which represent min cut

    // Re-run find levels
    auto find_levels_result = find_levels(r.edges, disks, left_border_x, right_border_x, config);
    auto levels = find_levels_result.levels;
    auto prev = find_levels_result.prev;

    std::vector<int> blocking_disks;

    // There are two gropus of disks
    // - disks where level(u_inbound) < inf and level(u_outbound) = inf
    // - disks where level(u_inbound) = inf and level(prev(u_inbound)) < inf
    for (unsigned int i = 0; i < disks.size(); i++) {
        const auto &disk = disks[i];
        TransformedVertex u_inbound = {disk.get_index(), true};

        // Check if u_inbound is reachable
        if (levels.contains(u_inbound)) {
            // Then u_outbound should not be reachable
            TransformedVertex u_outbound = {disk.get_index(), false};
            if (!levels.contains(u_outbound)) {
                // This is blocking disk
                blocking_disks.push_back(i);
            }
        } else {
            // u_inbound is unreachable. Then prev(u_inbound) should be reachable
            if (prev.contains(u_inbound) && levels.contains(prev[u_inbound])) {
                // Found blocking disk
                blocking_disks.push_back(i);
            }
        }
    }

    // Check that number of found disks is the same as number of blocking paths
    assert(blocking_disks.size() == r.path_count);

    return blocking_disks;
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

template std::vector<int> barrier_resilience_disks<int>(std::vector<Disk<int>> &disks,
                                                        const int &left_border_x,
                                                        const int &right_border_x,
                                                        const Config<int> &config);

template std::vector<int> barrier_resilience_disks<double>(std::vector<Disk<double>> &disks,
                                                           const double &left_border_x,
                                                           const double &right_border_x,
                                                           const Config<double> &config);