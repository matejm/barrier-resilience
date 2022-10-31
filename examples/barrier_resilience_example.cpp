#include <vector>
#include <iostream>
#include <fstream>
#include "geometry_objects.hpp"
#include "barrier_resillience.hpp"
#include "visualize.hpp"

int main() {
    auto disks = std::vector<Disk<int>>{
        {{1, 1}, 1},
        {{2, 2}, 1},
    };

    int left = 0,
        right = 3;

    std::ofstream SvgFile("preview.svg");
    SvgFile << problem_to_svg(disks, left, right) << std::endl;
    SvgFile.close();

    std::cout << barrier_resilience(disks, left, right) << std::endl;

}