#include <vector>
#include <iostream>
#include <fstream>
#include "utils/geometry_objects.hpp"
#include "with_graph_construction/barrier_resillience.hpp"
#include "utils/visualize.hpp"

int main() {
    auto disks = std::vector<Disk<int>>{
            {{1,  1},  1},
            {{2,  2},  1},
            {{3,  3},  1},
            {{2,  1},  1},
            {{0,  7},  2},
            {{2,  4},  1},
            {{0,  2},  1},
            {{0,  3},  1},
            {{4,  5},  2},
            {{5,  4},  2},
            {{6,  8},  4},
            {{3,  9},  1},
            {{2,  10}, 1},
            {{0,  9},  1},
            {{5,  0},  2},
            {{3,  -1}, 1},
            {{2,  -2}, 1},
            {{-1, -1}, 2},
            {{6,  -3}, 3},
            {{1,  -4}, 2},
            {{0,  -3}, 2},
            {{0,  12}, 1},
            {{2,  11}, 1},
            {{4,  14}, 1},
            {{1,  13}, 2},
            {{2,  11}, 2},
    };

    int left = 0,
            right = 5;

    std::cout << barrier_resilience_number_of_disks(disks, left, right) << std::endl;

    auto to_remove = barrier_resilience_disks(disks, left, right);

    std::cout << "To remove: ";
    for (int i: to_remove) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::ofstream SvgFile("preview.svg");
    SvgFile << problem_to_svg(disks, left, right, to_remove) << std::endl;
    SvgFile.close();
}