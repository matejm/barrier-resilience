#include <vector>
#include <iostream>
#include <fstream>
#include "utils/geometry_objects.hpp"
#include "utils/visualize.hpp"
#include "barrier_resilience/barrier_resilience.hpp"
#include "barrier_resilience/config.hpp"

int main() {
    const auto config = Config<int>::with_trivial_datastructure();

    auto disks = std::vector<Disk<int>>{
            {{1, -1}, 2},
            {{1, 1},  2},
            {{4, 0},  2},
    };

    int left = 0, right = 5;

    std::cout << barrier_resilience_number_of_disks<int>(disks, left, right, config)
              << std::endl;

    auto to_remove = barrier_resilience_disks(disks, left, right, config);

    std::cout << "To remove: ";
    for (unsigned int i = 0; i < to_remove.size(); ++i) {
        std::cout << to_remove[i] << " ";
    }
    std::cout << std::endl;

    std::ofstream SvgFile("preview.svg");
    SvgFile << problem_to_svg(disks, left, right, to_remove) << std::endl;
    SvgFile.close();

    return 0;
}