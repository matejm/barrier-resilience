#include <gtest/gtest.h>
#include <algorithm>
#include <fstream>
#include "barrier_resilience/barrier_resilience.hpp"
#include "with_graph_construction/graph_barrier_resilience.hpp"
#include "utils/geometry_objects.hpp"
#include "utils/visualize.hpp"


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

TEST(TestBarrierResilience, TestDisksParallelPaths) {
    const auto config = Config<int>::with_trivial_datastructure();

    int left_border_x = 0, right_border_x = 10;
    std::vector<Disk<int>> disks = {
            // First path
            {{1, 0},   10},
            // Second path
            {{1, 100}, 10},
    };

    ASSERT_EQ(barrier_resilience_number_of_disks(disks, left_border_x, right_border_x, config), 2);
    ASSERT_EQ(barrier_resilience_disks(disks, left_border_x, right_border_x, config), (std::vector<int>({0, 1})));

    left_border_x = 0, right_border_x = 10;
    disks = {
            // First path
            {{0, 0},  3},
            {{3, 0},  3},
            {{6, 0},  3},
            {{9, 0},  3},
            // Second path
            {{0, 10}, 3},
            {{3, 10}, 3},
            {{6, 10}, 3},
            {{9, 10}, 3},
    };

    ASSERT_EQ(barrier_resilience_number_of_disks(disks, left_border_x, right_border_x, config), 2);

    // Test edge case:
    right_border_x = 5;
    // multiple possible paths, disks are on the same line, but paths in single line are:
    // - only disk 1
    // - disks 0 -> disk 1
    // - disks 0 -> disk 2
    // We need to remove two disks on each path, so we need to remove 4 disks in total.
    ASSERT_EQ(barrier_resilience_number_of_disks(disks, left_border_x, right_border_x, config), 4);
}

// Test bug I found in my implementation on a random test case.
// Helped me to find a bug in calculating direct sum of edges.
TEST(TestBarrierResilience, TestDisksBugfixJoinEdges) {
    const auto config = Config<int>::with_trivial_datastructure();

    int left_border_x = 0, right_border_x = 10;
    std::vector<Disk<int>> disks = {
            {{1, 38}, 5},
            {{8, 7},  6},
            {{6, 38}, 8}, //
            {{2, 35}, 5},
            {{7, 16}, 5},
            {{3, 29}, 3}, //
            {{9, 30}, 6}, //
            {{1, 27}, 7},
            {{1, 33}, 7}, //
            {{6, 21}, 4},
            {{9, 17}, 6}
    };

    ASSERT_EQ(barrier_resilience_number_of_disks(disks, left_border_x, right_border_x, config), 3);
}

// Tests another bug I found in my implementation on a random test case.
// Bug was not considering that we need to run DFS along edges of a tree and went to deep on wrong path.
TEST(TestBarrierResilience, TestDisksBug) {
    const auto config = Config<int>::with_trivial_datastructure();

    int left_border_x = 1, right_border_x = 13;
    std::vector<Disk<int>> disks = {
            {{1,  10}, 4},
            {{6,  9},  3},
            {{5,  6},  3},
            {{10, 13}, 3},
            {{37, 0},  2},
            {{34, 0},  1},
            {{10, 6},  2},
            {{12, 17}, 4},
            {{36, 0},  3},
            {{8,  17}, 3},
            {{8,  3},  3},
            {{2,  8},  3},
            {{12, 4},  3},
            {{8,  12}, 2},
            {{4,  11}, 4},
    };

    auto d = barrier_resilience_disks(disks, left_border_x, right_border_x, config);
    std::sort(d.begin(), d.end());
    ASSERT_EQ(d, std::vector<int>({0, 11, 14}));
}

TEST(TestBarrierResilience, TestMatchingWithSimplerImplementation) {
    const auto config = Config<int>::with_trivial_datastructure();

    auto random = []() { return rand() % 1234567; };

    // Test if responses from graph implementation always match responses from this implementation.
    // Returned disks can be different, that's why we are testing only for same number of disks.
    for (int problem_size = 1; problem_size < 50; problem_size++) {
        // 10 random tests
        for (int _ = 0; _ < 10; ++_) {
            int width = 1 + random() % 20;
            int height = 1 + random() % 20;
            int mod_r = 1 + random() % width;

            int left_border_x = random();
            int right_border_x = left_border_x + width;

            // Generate random collection of disks
            std::vector<Disk<int>> disks;
            for (int i = 0; i < problem_size; i++) {
                Point<int> center = {
                        left_border_x + random() % width,
                        random() % height
                };
                int radius = 1 + random() % mod_r;
                disks.emplace_back(center, radius);
            }

            // Solve problem with graph construction
            int sol1 = graph_barrier_resilience_number_of_disks(disks, left_border_x, right_border_x);
            int sol2 = barrier_resilience_number_of_disks(disks, left_border_x, right_border_x, config);

            if (sol1 != sol2) {
                // Re-run with disk output to see what's wrong
                auto disks1 = graph_barrier_resilience_disks(disks, left_border_x, right_border_x);
                auto disks2 = barrier_resilience_disks(disks, left_border_x, right_border_x, config);

                std::ofstream SvgFile1("preview1.svg"), SvgFile2("preview2.svg");

                SvgFile1 << problem_to_svg(disks, left_border_x, right_border_x, disks1) << std::endl;
                SvgFile1.close();

                SvgFile2 << problem_to_svg(disks, left_border_x, right_border_x, disks2) << std::endl;
                SvgFile2.close();

                ASSERT_EQ(sol1, sol2) << "Solutions are different. Problem size: " << problem_size << std::endl
                                      << "Problem outputs were saved to preview1.svg and preview2.svg" << std::endl
                                      << "Problem: " << std::endl
                                      << "left_border_x = " << left_border_x
                                      << ", right_border_x = " << right_border_x << std::endl
                                      << "Disks: " << disks << std::endl;

            }
        }
    }
}