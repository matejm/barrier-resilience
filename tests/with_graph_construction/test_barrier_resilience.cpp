#include <gtest/gtest.h>

#include "with_graph_construction/barrier_resillience.hpp"
#include "utils/geometry_objects.hpp"

const std::vector<Algorithm> algorithms = {
        Algorithm::FordFulkerson,
        Algorithm::EvenTarjan,
};

TEST(TestBarrierResilience, TestNumberOfDisks) {
    for (auto a: algorithms) {
        auto disks = std::vector<Disk<int>>{
                {{0, 0}, 1},
                {{1, 1}, 1},
                {{2, 2}, 1},
                {{3, 3}, 1},
        };

        // Any of 4 disks can be removed
        ASSERT_EQ(barrier_resilience_number_of_disks(disks, 0, 3, a), 1);
        ASSERT_EQ(barrier_resilience_number_of_disks(disks, -1, 3, a), 1);
        ASSERT_EQ(barrier_resilience_number_of_disks(disks, 0, 4, a), 1);

        // Empty space to the left
        ASSERT_EQ(barrier_resilience_number_of_disks(disks, -2, 3, a), 0);
        // Empty space to the right
        ASSERT_EQ(barrier_resilience_number_of_disks(disks, 0, 5, a), 0);
        // Empty space to the left and right
        ASSERT_EQ(barrier_resilience_number_of_disks(disks, -2, 5, a), 0);
        // No disks
        ASSERT_EQ(barrier_resilience_number_of_disks({}, 0, 0, a), 0);

        disks = std::vector<Disk<int>>{
                {{1, -1}, 2},
                {{1, 1},  2},
                {{4, 0},  2},
        };

        // Possible to remove 1 disk
        ASSERT_EQ(barrier_resilience_number_of_disks(disks, 0, 5, a), 1);
        // Move right barrier to the left - need to remove 2 disks
        ASSERT_EQ(barrier_resilience_number_of_disks(disks, 0, 3, a), 2);

        // Add one large disk
        disks.push_back({{0, 0}, 10});
        ASSERT_EQ(barrier_resilience_number_of_disks(disks, 0, 5, a), 2);
        // Move right barrier to the left - need to remove 3 disks
        ASSERT_EQ(barrier_resilience_number_of_disks(disks, 0, 3, a), 3);

        // Add one more large disk
        disks.push_back({{0, 0}, 20});
        ASSERT_EQ(barrier_resilience_number_of_disks(disks, 0, 5, a), 3);
        // Move right barrier to the left - need to remove 4 disks
        ASSERT_EQ(barrier_resilience_number_of_disks(disks, 0, 3, a), 4);
        // Move right barrier to the right - need to remove only two large disks
        ASSERT_EQ(barrier_resilience_number_of_disks(disks, 0, 8, a), 2);
        // Move right barrier to the right - need to remove only one large disk
        ASSERT_EQ(barrier_resilience_number_of_disks(disks, 0, 15, a), 1);
        // Move right barrier to the right - no disks to remove
        ASSERT_EQ(barrier_resilience_number_of_disks(disks, 0, 25, a), 0);
    }
}

TEST(TestBarrierResilience, TestDisks) {
    for (auto a: algorithms) {

        auto disks = std::vector<Disk<int>>{
                {{1, -1}, 2},
                {{1, 1},  2},
                {{4, 0},  2},
        };

        // Possible to remove 1 disk
        ASSERT_EQ(barrier_resilience_disks(disks, 0, 5, a), (std::vector<int>({2})));
        // Move right barrier to the left - need to remove 2 disks
        ASSERT_EQ(barrier_resilience_disks(disks, 0, 3, a), (std::vector<int>({0, 1})));

        // Add one large disk
        disks.push_back({{0, 0}, 10});
        ASSERT_EQ(barrier_resilience_disks(disks, 0, 5, a), (std::vector<int>({2, 3})));
        // Move right barrier to the left - need to remove 3 disks
        ASSERT_EQ(barrier_resilience_disks(disks, 0, 3, a), (std::vector<int>({0, 1, 3})));

        // Add one more large disk
        disks.push_back({{0, 0}, 20});
        ASSERT_EQ(barrier_resilience_disks(disks, 0, 5, a), (std::vector<int>({2, 3, 4})));
        // Move right barrier to the left - need to remove 4 disks
        ASSERT_EQ(barrier_resilience_disks(disks, 0, 3, a), (std::vector<int>({0, 1, 3, 4})));
        // Move right barrier to the right - need to remove only two large disks
        ASSERT_EQ(barrier_resilience_disks(disks, 0, 8, a), (std::vector<int>({3, 4})));
        // Move right barrier to the right - need to remove only one large disk
        ASSERT_EQ(barrier_resilience_disks(disks, 0, 15, a), (std::vector<int>({4})));
        // Move right barrier to the right - no disks to remove
        ASSERT_EQ(barrier_resilience_disks(disks, 0, 25, a), (std::vector<int>()));

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
        ASSERT_EQ(barrier_resilience_number_of_disks(disks, 0, 5, a), 3);
        ASSERT_EQ(barrier_resilience_disks(disks, 0, 5, a), (std::vector<int>({1, 10, 16})));

        ASSERT_EQ(barrier_resilience_number_of_disks(disks, 0, 4, a), 4);
        ASSERT_EQ(barrier_resilience_disks(disks, 0, 4, a), (std::vector<int>({1, 16, 24, 25})));

        ASSERT_EQ(barrier_resilience_number_of_disks(disks, 1, 4, a), 6);
        ASSERT_EQ(barrier_resilience_disks(disks, 1, 4, a), (std::vector<int>({5, 12, 1, 16, 22, 25})));

        ASSERT_EQ(barrier_resilience_number_of_disks(disks, 0, 10, a), 1);
        ASSERT_EQ(barrier_resilience_disks(disks, 0, 10, a), (std::vector<int>({10})));
    }
}