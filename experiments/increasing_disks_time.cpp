#include <iostream>
#include "barrier_resilience/config.hpp"
#include "with_graph_construction/graph_barrier_resilience.hpp"
#include "helpers.hpp"

int main() {
    const auto config_trivial = Config<int>::with_trivial_datastructure();
    const auto config_kdtree = Config<int>::with_kdtree();

    auto timer = Timer();

    for (int i = 0; i < 10001; i += 100) {
        timer.start();

        int result_trivial = evalutate_problem({10, 0, 100, 0, 100, i}, config_trivial);

        double time_trivial = timer.time_elapsed();
        timer.start();

        int result_kdtree = evalutate_problem({10, 0, 100, 0, 100, i}, config_kdtree);

        double time_kdtree = timer.time_elapsed();

        assert(result_trivial == result_kdtree);

        timer.start();

        int result_ford_fulkerson = evalutate_problem({10, 0, 100, 0, 100, i}, Algorithm::FordFulkerson);

        double time_ford_fulkerson = timer.time_elapsed();

        assert(result_trivial == result_ford_fulkerson);

        timer.start();

        int result_even_tarjan = evalutate_problem({10, 0, 100, 0, 100, i}, Algorithm::EvenTarjan);

        double time_even_tarjan = timer.time_elapsed();

        assert(result_trivial == result_even_tarjan);

        std::cout << i << "," << time_trivial << "," << time_kdtree << "," << time_ford_fulkerson << ","
                  << time_even_tarjan << std::endl;
    }

    return 0;
}