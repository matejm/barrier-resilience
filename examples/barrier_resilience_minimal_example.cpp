#include <vector>
#include <iostream>
#include <fstream>
#include "geometry_objects.hpp"
#include "barrier_resillience.hpp"
#include "visualize.hpp"

int main() {
    auto disks = std::vector<Disk<int>>{
        {{1, -1}, 2},
        {{1, 1}, 2},
        {{4, 0}, 2},
    };

    int left = 0,
        right = 5;

    std::cout << barrier_resilience_number_of_disks(disks, left, right) << std::endl;

    auto to_remove = barrier_resilience_disks(disks, left, right);

    std::cout << "To remove: ";
    for (int i = 0; i < to_remove.size(); ++i) {
        std::cout << to_remove[i] << " ";
    }
    std::cout << std::endl;

    std::ofstream SvgFile("preview.svg");
    SvgFile << problem_to_svg(disks, left, right, to_remove) << std::endl;
    SvgFile.close();

    return 0;
}