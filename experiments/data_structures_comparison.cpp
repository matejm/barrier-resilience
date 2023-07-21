#include <iostream>
#include <chrono>
#include <tuple>
//#include "data_structure/trivial.hpp"
#include "data_structure/kdtree.hpp"
#include "helpers.hpp"

const int N = 1000000;
const int SIZE = 1000;

int main() {

    // Compare speed of trivial data structure and kdtree.
    auto trivial = Trivial<int>();
    auto kdtree = KDTree<int>();

    auto timer = Timer();

    // Generate 1000 disks.
    auto disks = std::vector<GeometryObject<int>>();
    for (int i = 0; i < N; ++i) {
        disks.push_back(Disk<int>{{rand() % SIZE, rand() % SIZE}, 1});

        // Every 100000 disks, rebuild the data structure
        if (i % 10000 == 0) {
            timer.start();

            // Generate data structure on current disks.
            trivial.rebuild(disks);

            double trivial_construction_time = timer.time_elapsed();
            timer.start();

            // Perform 1000 random queries.
            for (int i = 0; i < 1000; ++i) {
                trivial.intersecting(Disk<int>{{rand() % SIZE, rand() % SIZE}, 1});
            }

            double trivial_query_time = timer.time_elapsed();
            timer.start();

            // Generate data structure on current disks.
            kdtree.rebuild(disks);

            double kdtree_construction_time = timer.time_elapsed();
            timer.start();

            // Perform 1000 random queries.
            for (int i = 0; i < 1000; ++i) {
                kdtree.intersecting(Disk<int>{{rand() % SIZE, rand() % SIZE}, 1});
            }

            double kdtree_query_time = timer.time_elapsed();

            std::cout << i << "," << trivial_construction_time << "," << trivial_query_time << ","
                      << kdtree_construction_time << "," << kdtree_query_time << std::endl;
        }

    }


    return 0;
}