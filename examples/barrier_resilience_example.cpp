#include <vector>
#include <iostream>
#include "geometry_objects.hpp"
#include "barrier_resillience.hpp"

int main() {
    auto disks = std::vector<Disk<int>>{
        {{0, 0}, 1},
        {{1, 1}, 1},
        {{2, 2}, 2},
        {{3, 3}, 1},
        {{4, 4}, 1},
    };

    barrier_resilience(disks, -1, 5);


}