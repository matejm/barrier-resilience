#include <iostream>
#include "barrier_resilience/config.hpp"
#include "helpers.hpp"

int main() {
    const auto config = Config<int>::with_trivial_datastructure();

    // Evaluate 5-times and take the average
    const auto repeats = 5;

    for (int i = 0; i < 1001; ++i) {
        int result = 0;
        for (int j = 0; j < repeats; ++j) {
            result += evalutate_problem({10, 0, 100, 0, 100, i}, config);
        }

        result /= repeats;

        std::cout << i << "," << result << std::endl;
    }

    return 0;
}