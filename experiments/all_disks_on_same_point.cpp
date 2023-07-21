#include <cassert>
#include <iostream>
#include "barrier_resilience/config.hpp"
#include "with_graph_construction/graph_barrier_resilience.hpp"
#include "helpers.hpp"

int main() {
    const auto config_trivial = Config<int>::with_trivial_datastructure();
    const auto config_kdtree = Config<int>::with_kdtree();

    for (int i = 1; i < 10001; i += 100) {
        const ProblemParams params = {1, 0, 1, 0, 1, i};


        auto results = compare_algorithms(params,
                                          {config_trivial, config_kdtree},
                                          {}
        );

        std::cout << i << "," << results[0] << "," << results[1] << "," << results[2] << "," << results[3] << std::endl;
    }

    return 0;
}