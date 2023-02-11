#include <vector>
#include <iostream>
#include <fstream>
#include "utils/geometry_objects.hpp"
#include "with_graph_construction/graph_barrier_resilience.hpp"
#include "utils/visualize.hpp"
#include "barrier_resilience/barrier_resilience.hpp"
#include "barrier_resilience/config.hpp"

int main() {
    auto disks = std::vector<Disk<int>>{
            {{1, -1}, 2},
            {{1, 1},  2},
            {{4, 0},  2},
    };

    int left = 0, right = 5;

    std::cout << graph_barrier_resilience_number_of_disks(disks, left, right) << std::endl;
    std::cout << barrier_resilience_number_of_disks<int>(disks, left, right,
                                                         Config<int>::with_trivial_datastructure())
              << std::endl;

    auto to_remove = graph_barrier_resilience_disks(disks, left, right);

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