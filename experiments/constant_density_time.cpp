#include <iostream>
#include "barrier_resilience/config.hpp"
#include "with_graph_construction/graph_barrier_resilience.hpp"
#include "helpers.hpp"

int main() {
    const auto config_trivial = Config<int>::with_trivial_datastructure();
    const auto config_kdtree = Config<int>::with_kdtree();

    for (int i = 0; i < 100000; i += 100) {
        auto params = ProblemParams{10, 0, 100, 0, i / 10, i};

        auto times = compare_algorithms(params, {config_trivial, config_kdtree},
                                        {Algorithm::FordFulkerson}
        );

        std::cout << i << "," << times[0] << "," << times[1] << "," << times[2] << std::endl;
    }

    return 0;
}