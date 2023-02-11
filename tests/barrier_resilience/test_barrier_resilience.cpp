#include <gtest/gtest.h>

#include "barrier_resilience/barrier_resilience.hpp"
#include "utils/geometry_objects.hpp"
#include "data_structure/trivial.hpp"

TEST(TestBarrierResilience, TestNumberOfDisks) {
    const auto config = Config<int>::with_trivial_datastructure();

    auto disks = std::vector<Disk<int>>{
            {{0, 0}, 1},
            {{1, 1}, 1},
            {{2, 2}, 1},
            {{3, 3}, 1},
    };

    // Any of 4 disks can be removed
    ASSERT_EQ((barrier_resilience_number_of_disks<int>(disks, 0, 3, config)), 1);
    ASSERT_EQ((barrier_resilience_number_of_disks<int>(disks, -1, 3, config)), 1);
    ASSERT_EQ((barrier_resilience_number_of_disks<int>(disks, 0, 4, config)), 1);

    // Empty space to the left
    ASSERT_EQ((barrier_resilience_number_of_disks<int>(disks, -2, 3, config)), 0);
    // Empty space to the right
    ASSERT_EQ((barrier_resilience_number_of_disks<int>(disks, 0, 5, config)), 0);
    // Empty space to the left and right
    ASSERT_EQ((barrier_resilience_number_of_disks<int>(disks, -2, 5, config)), 0);
    // No disks
    auto no_disks = std::vector<Disk<int>>();
    ASSERT_EQ((barrier_resilience_number_of_disks<int>(no_disks, 0, 0, config)), 0);

    disks = std::vector<Disk<int>>{
            {{1, -1}, 2},
            {{1, 1},  2},
            {{4, 0},  2},
    };

    // Possible to remove 1 disk
    ASSERT_EQ((barrier_resilience_number_of_disks<int>(disks, 0, 5, config)), 1);
    // Move right barrier to the left - need to remove 2 disks
    ASSERT_EQ((barrier_resilience_number_of_disks<int>(disks, 0, 3, config)), 2);

    // Add one large disk
    disks.push_back({{0, 0}, 10});
    ASSERT_EQ((barrier_resilience_number_of_disks<int>(disks, 0, 5, config)), 2);
    // Move right barrier to the left - need to remove 3 disks
    ASSERT_EQ((barrier_resilience_number_of_disks<int>(disks, 0, 3, config)), 3);

    // Add one more large disk
    disks.push_back({{0, 0}, 20});
    ASSERT_EQ((barrier_resilience_number_of_disks<int>(disks, 0, 5, config)), 3);
    // Move right barrier to the left - need to remove 4 disks
    ASSERT_EQ((barrier_resilience_number_of_disks<int>(disks, 0, 3, config)), 4);
    // Move right barrier to the right - need to remove only two large disks
    ASSERT_EQ((barrier_resilience_number_of_disks<int>(disks, 0, 8, config)), 2);
    // Move right barrier to the right - need to remove only one large disk
    ASSERT_EQ((barrier_resilience_number_of_disks<int>(disks, 0, 15, config)), 1);
    // Move right barrier to the right - no disks to remove
    ASSERT_EQ((barrier_resilience_number_of_disks<int>(disks, 0, 25, config)), 0);
}
