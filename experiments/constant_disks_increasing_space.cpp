#include <iostream>
#include "barrier_resilience/config.hpp"
#include "helpers.hpp"

int main() {
    const auto config = Config<int>::with_trivial_datastructure();

    // Evaluate 5-times and take the average
    const auto repeats = 5;

    for (int i = 1; i < 1001; i++) {
        int result_vertical = 0, result_horizontal = 0;

        for (int j = 0; j < repeats; j++) {
            // Increase space vertically.
            result_vertical += evalutate_problem({10, 0, 100, 0, i, 1000}, config);

            // Increase space horizontally.
            result_horizontal += evalutate_problem({10, 0, i, 0, 100, 1000}, config);
        }

        result_vertical /= repeats;
        result_horizontal /= repeats;

        std::cout << static_cast<double>(i) / 10.0 << "," << result_vertical << "," << result_horizontal << std::endl;
    }

    return 0;
}