#include <iostream>
#include "barrier_resilience/config.hpp"
#include "helpers.hpp"

int main() {
    const auto config = Config<int>::with_trivial_datastructure();

    for (int i = 0; i < 1001; ++i) {
        auto result = evalutate_problem({10, 0, 100, 0, 100, i}, config);
        std::cout << i << "," << result << std::endl;
    }

    return 0;
}