#include <vector>
#include <iostream>
#include "geometry_objects.hpp"

int main() {
    auto disks = std::vector<Disk<int>>{
        {{0, 0}, 1},
        {{1, 1}, 1},
        {{2, 2}, 2},
        {{3, 3}, 1},
        {{4, 4}, 1},
    };

    for (int i = 0; i < disks.size(); ++i) {
        for (int j = i + 1; j < disks.size(); ++j) {
            if (intersects(disks[i], disks[j])) {
                std::cout << "Disk " << i << " intersects disk " << j << std::endl;
            }
        }
    }

    return 0;
}