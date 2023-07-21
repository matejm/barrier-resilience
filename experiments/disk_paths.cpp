#include <iostream>
#include <algorithm>
#include "barrier_resilience/config.hpp"
#include "with_graph_construction/graph_barrier_resilience.hpp"
#include "helpers.hpp"

int main() {
    const auto config_trivial = Config<int>::with_trivial_datastructure();
    const auto config_kdtree = Config<int>::with_kdtree();

    for (int i = 10000; i < 10001; i++) {
        const ProblemParams params = {1, 0, 10000, 0, 1, i};

        // Generate paths of disks
        std::vector<Disk<int>> disks;
        // Each disk will be independent path
        for (int j = 0; j < i; ++j) {
            disks.push_back({{i, 0}, 1});
        }

        // Reverse the disks
        std::reverse(disks.begin(), disks.end());

        auto results = compare_algorithms(params,
                                          {config_trivial, config_kdtree},
                                          {},
                                          disks
        );

        std::cout << i << "," << results[0] << "," << results[1] << "," << results[2] << "," << results[3] << std::endl;
    }

    return 0;
}