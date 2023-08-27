#include <iostream>
#include "barrier_resilience/config.hpp"
#include "helpers.hpp"

int main() {
    const auto config = Config<int>::with_trivial_datastructure();

    // Evaluate 5-times and take the average
    const auto repeats = 5;

    for (int i = 1; i < 1001; i++) {
        // Increase the space by 1 each iteration, distance = number of disks.
        int result = 0;

        for (int j = 0; j < repeats; j++) {
            result += evalutate_problem({i, 0, 1000, 0, 1000, 1000}, config);
        }

        result /= repeats;

        std::cout << static_cast<double>(i) / 100.0 << "," << result << std::endl;
    }

    return 0;
}