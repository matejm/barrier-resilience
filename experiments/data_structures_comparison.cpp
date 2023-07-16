#include <iostream>
#include <chrono>
#include "data_structure/trivial.hpp"
#include "data_structure/kdtree.hpp"
#include "helpers.hpp"

const int N = 10000;

int main() {
    // Compare speed of trivial data structure and kdtree.
    auto trivial = Trivial<int>();
    auto kdtree = KDTree<int>();

    auto timer = Timer();

    // Generate 1000 disks.
    auto disks = std::vector<GeometryObject<int>>();
    for (int i = 0; i < N; ++i) {
        disks.push_back(Disk<int>{{i, i}, 1});

        // Every 10 disks, rebuild the data structure

        if (i % 10 == 0) {
            timer.start();

            // Generate data structure on current disks.
            kdtree.rebuild(disks);

            std::cout << i << "," << timer.time_elapsed() << std::endl;
        }
    }

    return 0;
}