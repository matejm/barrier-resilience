#include <iostream>
#include "barrier_resilience/config.hpp"
#include "helpers.hpp"

int main() {
    const auto config = Config<int>::with_trivial_datastructure();

    for (int i = 1; i < 1001; i++) {
        // Increase the space by 1 each iteration, distance = number of disks.
        auto result = evalutate_problem({i, 0, 1000, 0, 1000, 1000}, config);
        std::cout << static_cast<double>(i) / 100.0 << "," << result << std::endl;
    }

    return 0;
}