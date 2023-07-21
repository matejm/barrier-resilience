#include <iostream>
#include "barrier_resilience/config.hpp"
#include "helpers.hpp"

int main() {
    const auto config = Config<int>::with_trivial_datastructure();

    for (int i = 1; i < 1001; i++) {
        // Increase space vertically.
        int result_vertical = evalutate_problem({10, 0, 100, 0, i, 1000}, config);

        // Increase space horizontally.
        int result_horizontal = evalutate_problem({10, 0, i, 0, 100, 1000}, config);

        std::cout << static_cast<double>(i) / 10.0 << "," << result_vertical << "," << result_horizontal << std::endl;
    }

    return 0;
}