#include <gtest/gtest.h>
#include <algorithm>
#include "barrier_resilience/barrier_resilience.hpp"
#include "with_graph_construction/graph_barrier_resilience.hpp"
#include "utils/geometry_objects.hpp"

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


TEST(TestBarrierResilience, TestDisks) {
    const auto config = Config<int>::with_trivial_datastructure();

    auto disks = std::vector<Disk<int>>{
            {{1, -1}, 2},
            {{1, 1},  2},
            {{4, 0},  2},
    };

    // Possible to remove 1 disk
    ASSERT_EQ(barrier_resilience_disks(disks, 0, 5, config), (std::vector<int>({2})));
    // Move right barrier to the left - need to remove 2 disks
    ASSERT_EQ(barrier_resilience_disks(disks, 0, 3, config), (std::vector<int>({0, 1})));

    // Add one large disk
    disks.push_back({{0, 0}, 10});
    ASSERT_EQ(barrier_resilience_disks(disks, 0, 5, config), (std::vector<int>({2, 3})));
    // Move right barrier to the left - need to remove 3 disks
    ASSERT_EQ(barrier_resilience_disks(disks, 0, 3, config), (std::vector<int>({0, 1, 3})));

    // Add one more large disk
    disks.push_back({{0, 0}, 20});
    ASSERT_EQ(barrier_resilience_disks(disks, 0, 5, config), (std::vector<int>({2, 3, 4})));
    // Move right barrier to the left - need to remove 4 disks
    ASSERT_EQ(barrier_resilience_disks(disks, 0, 3, config), (std::vector<int>({0, 1, 3, 4})));
    // Move right barrier to the right - need to remove only two large disks
    ASSERT_EQ(barrier_resilience_disks(disks, 0, 8, config), (std::vector<int>({3, 4})));
    // Move right barrier to the right - need to remove only one large disk
    ASSERT_EQ(barrier_resilience_disks(disks, 0, 15, config), (std::vector<int>({4})));
    // Move right barrier to the right - no disks to remove
    ASSERT_EQ(barrier_resilience_disks(disks, 0, 25, config), (std::vector<int>()));

    // Non-trivial case
    disks = std::vector<Disk<int>>{
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

    // There might be multiple solutions, but all of them should have the same number of disks.
    ASSERT_EQ(barrier_resilience_number_of_disks(disks, 0, 5, config), 3);
    ASSERT_EQ(barrier_resilience_disks(disks, 0, 5, config), (std::vector<int>({1, 10, 16})));

    ASSERT_EQ(barrier_resilience_number_of_disks(disks, 0, 4, config), 4);
    ASSERT_EQ(barrier_resilience_disks(disks, 0, 4, config), (std::vector<int>({1, 16, 24, 25})));

    ASSERT_EQ(barrier_resilience_number_of_disks(disks, 1, 4, config), 6);
    // Sort to be able to compare
    auto to_remove = barrier_resilience_disks(disks, 1, 4, config);
    std::sort(to_remove.begin(), to_remove.end());
    ASSERT_EQ(to_remove, (std::vector<int>({1, 5, 12, 16, 22, 25})));

    ASSERT_EQ(barrier_resilience_number_of_disks(disks, 0, 10, config), 1);
    ASSERT_EQ(barrier_resilience_disks(disks, 0, 10, config), (std::vector<int>({10})));
}
