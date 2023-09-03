#include <iostream>
#include "barrier_resilience/config.hpp"
#include "with_graph_construction/graph_barrier_resilience.hpp"
#include "helpers.hpp"

int main() {
    const auto config_trivial = Config<int>::with_trivial_datastructure();
    const auto config_kdtree = Config<int>::with_kdtree();

    // Evaluate 5-times and take the average
    const auto repeats = 5;

    for (int i = 0; i < 5001; i += 100) {

        double times_trivial = 0, times_kdtree = 0, times_ff = 0;

        for (int j = 0; j < repeats; j++) {
            auto params = ProblemParams{10, 0, 100, 0, 100, i};

            auto times = compare_algorithms(params, {config_trivial, config_kdtree},
                                            {Algorithm::FordFulkerson}
            );

            times_trivial += times[0];
            times_kdtree += times[1];
            times_ff += times[2];
        }

        std::cout << i << "," << times_trivial / repeats << "," << times_kdtree / repeats << "," << times_ff / repeats
                  << std::endl;
    }

    return 0;
}