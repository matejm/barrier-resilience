#include <gtest/gtest.h>
#include "data_structure/trivial.hpp"
#include "find_levels.hpp"
#include <vector>

TEST(TestFindLevels, TestEmptyPaths) {
    const auto paths = std::vector<Path>{};

    // Trivial disk layout
    auto disks = std::vector<Disk<int>>{
            {{0, 0}, 1},
            {{1, 1}, 1},
    };
    int left_border = -1;
    int right_border = 2;

    auto r = find_levels<int, Trivial>(paths, disks, left_border, right_border);
    ASSERT_TRUE(r.reachable);

    // Expect levels for vertices s, t and 4  for disks
    ASSERT_EQ(r.levels.size(), 6);
    // Expect level of s to be 0
    ASSERT_EQ(r.levels[source], 0);
    // L[1]
    ASSERT_EQ((r.levels[TransformedVertex{0, true}]), 1);
    // L[2]
    ASSERT_EQ((r.levels[TransformedVertex{0, false}]), 2);
    // L[3]
    ASSERT_EQ((r.levels[TransformedVertex{1, true}]), 3);
    // L[4]
    ASSERT_EQ((r.levels[TransformedVertex{1, false}]), 4);
    // Expect level of t to be 5
    ASSERT_EQ((r.levels[sink]), 5);
    ASSERT_EQ(r.distance, 5);

    // Another trivial disk layout
    disks = std::vector<Disk<int>>{
            {{0, 0}, 2},
            {{0, 3}, 2},
            {{2, 1}, 2},
    };
    left_border = -1;
    right_border = 3;

    r = find_levels<int, Trivial>(paths, disks, left_border, right_border);
    ASSERT_TRUE(r.reachable);

    // Expect levels for vertices s, t and 6 for disks
    ASSERT_EQ(r.levels.size(), 8);

    // Expect level of s to be 0
    ASSERT_EQ(r.levels[source], 0);
    // L[1]
    ASSERT_EQ((r.levels[TransformedVertex{0, true}]), 1);
    ASSERT_EQ((r.levels[TransformedVertex{1, true}]), 1);
    // L[2]
    ASSERT_EQ((r.levels[TransformedVertex{0, false}]), 2);
    ASSERT_EQ((r.levels[TransformedVertex{1, false}]), 2);
    // L[3]
    ASSERT_EQ((r.levels[TransformedVertex{2, true}]), 3);
    // L[4]
    ASSERT_EQ((r.levels[TransformedVertex{2, false}]), 4);
    // Expect level of t to be 5
    ASSERT_EQ((r.levels[sink]), 5);
    ASSERT_EQ(r.distance, 5);

    // Another layout
    disks = std::vector<Disk<int>>{
            {{0, 0}, 2},
            {{0, 5}, 2},
            {{2, 0}, 2},
            {{2, 5}, 2},
            {{5, 0}, 2},
            {{5, 5}, 2},
    };
    left_border = -1;
    right_border = 6;

    r = find_levels<int, Trivial>(paths, disks, left_border, right_border);
    ASSERT_TRUE(r.reachable);

    // Expect levels for vertices s, t and 12 for disks
    ASSERT_EQ(r.levels.size(), 14);

    // Expect level of s to be 0
    ASSERT_EQ(r.levels[source], 0);
    // L[1]
    ASSERT_EQ((r.levels[TransformedVertex{0, true}]), 1);
    ASSERT_EQ((r.levels[TransformedVertex{1, true}]), 1);
    // L[2]
    ASSERT_EQ((r.levels[TransformedVertex{0, false}]), 2);
    ASSERT_EQ((r.levels[TransformedVertex{1, false}]), 2);
    // L[3]
    ASSERT_EQ((r.levels[TransformedVertex{2, true}]), 3);
    ASSERT_EQ((r.levels[TransformedVertex{3, true}]), 3);
    // L[4]
    ASSERT_EQ((r.levels[TransformedVertex{2, false}]), 4);
    ASSERT_EQ((r.levels[TransformedVertex{3, false}]), 4);
    // L[5]
    ASSERT_EQ((r.levels[TransformedVertex{4, true}]), 5);
    ASSERT_EQ((r.levels[TransformedVertex{5, true}]), 5);
    // L[6]
    ASSERT_EQ((r.levels[TransformedVertex{4, false}]), 6);
    ASSERT_EQ((r.levels[TransformedVertex{5, false}]), 6);
    // Expect level of t to be 7
    ASSERT_EQ((r.levels[sink]), 7);
    ASSERT_EQ(r.distance, 7);

    // Another layout
    disks = std::vector<Disk<int>>{
            // Longer path
            {{1, 0},  1},
            {{3, 0},  1},
            {{3, 2},  1},
            {{3, 4},  1},
            {{5, 4},  1},
            // False branch
            {{3, -2}, 1},
            {{3, -4}, 1},
            {{3, -6}, 1},
    };
    left_border = 0;
    right_border = 6;

    r = find_levels<int, Trivial>(paths, disks, left_border, right_border);
    ASSERT_TRUE(r.reachable);

    // Expect levels for vertices s, t and 16 for disks
    ASSERT_EQ(r.levels.size(), 18);

    // Expect level of s to be 0
    ASSERT_EQ(r.levels[source], 0);
    // L[1]
    ASSERT_EQ((r.levels[TransformedVertex{0, true}]), 1);
    // L[2]
    ASSERT_EQ((r.levels[TransformedVertex{0, false}]), 2);
    // L[3]
    ASSERT_EQ((r.levels[TransformedVertex{1, true}]), 3);
    // L[4]
    ASSERT_EQ((r.levels[TransformedVertex{1, false}]), 4);
    // L[5] - branching
    ASSERT_EQ((r.levels[TransformedVertex{2, true}]), 5);
    ASSERT_EQ((r.levels[TransformedVertex{5, true}]), 5);
    // L[6]
    ASSERT_EQ((r.levels[TransformedVertex{2, false}]), 6);
    ASSERT_EQ((r.levels[TransformedVertex{5, false}]), 6);
    // L[7]
    ASSERT_EQ((r.levels[TransformedVertex{3, true}]), 7);
    ASSERT_EQ((r.levels[TransformedVertex{6, true}]), 7);
    // L[8]
    ASSERT_EQ((r.levels[TransformedVertex{3, false}]), 8);
    ASSERT_EQ((r.levels[TransformedVertex{6, false}]), 8);
    // L[9]
    ASSERT_EQ((r.levels[TransformedVertex{4, true}]), 9);
    ASSERT_EQ((r.levels[TransformedVertex{7, true}]), 9);
    // L[10]
    ASSERT_EQ((r.levels[TransformedVertex{4, false}]), 10);
    ASSERT_EQ((r.levels[TransformedVertex{7, false}]), 10);
    // Expect level of t to be 11
    ASSERT_EQ((r.levels[sink]), 11);
    ASSERT_EQ(r.distance, 11);

    // Another layout
    disks = std::vector<Disk<int>>{
            {{0, 0},  2},
            {{0, 5},  2},
            {{0, 15}, 3},
    };
    left_border = -3;
    right_border = 3;

    r = find_levels<int, Trivial>(paths, disks, left_border, right_border);
    ASSERT_TRUE(r.reachable);

    // Expect levels for vertices s, t and 2 for only reachable disk
    ASSERT_EQ(r.levels.size(), 4);

    // Expect level of s to be 0
    ASSERT_EQ(r.levels[source], 0);
    // L[1]
    ASSERT_EQ((r.levels[TransformedVertex{2, true}]), 1);
    // L[2]
    ASSERT_EQ((r.levels[TransformedVertex{2, false}]), 2);
    // Expect level of t to be 3
    ASSERT_EQ((r.levels[sink]), 3);
    ASSERT_EQ(r.distance, 3);
}

TEST(TestFindLevels, TestEmptyPathsLongerPathDropped) {
    // Check if longer path is dropped when we reach sink
    const auto paths = std::vector<Path>{};

    auto disks = std::vector<Disk<int>>{
            {{3, 15}, 3},
            {{1, 0},  1},
            {{3, 0},  1},
            {{5, 0},  1},
    };
    int left_border = 0;
    int right_border = 6;

    auto r = find_levels<int, Trivial>(paths, disks, left_border, right_border);
    ASSERT_TRUE(r.reachable);

    // Expect levels for vertices s, t and only 4 for disks
    // (because we drop longer path as soon as we reach sink)
    ASSERT_EQ(r.levels.size(), 6);

    // Expect level of s to be 0
    ASSERT_EQ(r.levels[source], 0);
    // L[1]
    ASSERT_EQ((r.levels[TransformedVertex{0, true}]), 1);
    ASSERT_EQ((r.levels[TransformedVertex{1, true}]), 1);
    // L[2]
    ASSERT_EQ((r.levels[TransformedVertex{0, false}]), 2);
    ASSERT_EQ((r.levels[TransformedVertex{1, false}]), 2);
    // Expect level of t to be 3. No other vertices should be present on the level
    ASSERT_EQ((r.levels[sink]), 3);
    ASSERT_EQ(r.distance, 3);

    // Another layout
    disks = std::vector<Disk<int>>{
            {{1, 0},  1},
            {{3, 0},  1},
            {{3, 2},  1},
            {{5, 2},  1},
            // False branch
            {{3, -2}, 1},
            {{3, -4}, 1},
            {{3, -6}, 1},
            {{3, -8}, 1},
    };
    left_border = 0;
    right_border = 6;

    r = find_levels<int, Trivial>(paths, disks, left_border, right_border);
    ASSERT_TRUE(r.reachable);

    // Expect levels for vertices s, t and 12 for disks
    // (because we drop longer path as soon as we reach sink)
    ASSERT_EQ(r.levels.size(), 14);

    // Expect level of s to be 0
    ASSERT_EQ(r.levels[source], 0);
    // L[1]
    ASSERT_EQ((r.levels[TransformedVertex{0, true}]), 1);
    // L[2]
    ASSERT_EQ((r.levels[TransformedVertex{0, false}]), 2);
    // L[3]
    ASSERT_EQ((r.levels[TransformedVertex{1, true}]), 3);
    // L[4]
    ASSERT_EQ((r.levels[TransformedVertex{1, false}]), 4);
    // L[5]
    ASSERT_EQ((r.levels[TransformedVertex{2, true}]), 5);
    ASSERT_EQ((r.levels[TransformedVertex{4, true}]), 5);
    // L[6]
    ASSERT_EQ((r.levels[TransformedVertex{2, false}]), 6);
    ASSERT_EQ((r.levels[TransformedVertex{4, false}]), 6);
    // L[7]
    ASSERT_EQ((r.levels[TransformedVertex{3, true}]), 7);
    ASSERT_EQ((r.levels[TransformedVertex{5, true}]), 7);
    // L[8]
    ASSERT_EQ((r.levels[TransformedVertex{3, false}]), 8);
    ASSERT_EQ((r.levels[TransformedVertex{5, false}]), 8);
    // Expect level of t to be 9. No other vertices should be present on the level
    ASSERT_EQ((r.levels[sink]), 9);
    ASSERT_EQ(r.distance, 9);
}

TEST(TestFindLevels, TestEmptyPathsUnreachable) {
    // Check if unreachable is detected correctly
    const auto paths = std::vector<Path>{};

    auto disks = std::vector<Disk<int>>{
            {{1, 0}, 1},
            {{3, 0}, 1},
            {{6, 0}, 1},
    };
    int left_border = 0;
    int right_border = 7;

    auto r = find_levels<int, Trivial>(paths, disks, left_border, right_border);
    ASSERT_FALSE(r.reachable);

    // Expect levels for vertex s and only 4 for disks
    ASSERT_EQ(r.levels.size(), 5);

    // Expect level of s to be 0
    ASSERT_EQ(r.levels[source], 0);
    // L[1]
    ASSERT_EQ((r.levels[TransformedVertex{0, true}]), 1);
    // L[2]
    ASSERT_EQ((r.levels[TransformedVertex{0, false}]), 2);
    // L[3]
    ASSERT_EQ((r.levels[TransformedVertex{1, true}]), 3);
    // L[4]
    ASSERT_EQ((r.levels[TransformedVertex{1, false}]), 4);
    // No level for t
    ASSERT_TRUE(r.levels.find(sink) == r.levels.end());
    ASSERT_EQ(r.distance, -1);

    // Another layout
    disks = std::vector<Disk<int>>{
            // False branch
            {{1, 0},  1},
            {{3, 0},  1},
            {{5, 0},  1},
            // False branch
            {{1, 10}, 1},
            {{3, 10}, 1},
            {{6, 10}, 1},
    };
    left_border = 0;
    right_border = 7;

    r = find_levels<int, Trivial>(paths, disks, left_border, right_border);
    ASSERT_FALSE(r.reachable);

    // Expect levels for vertex s and 10 for disks
    ASSERT_EQ(r.levels.size(), 11);

    // Expect level of s to be 0
    ASSERT_EQ(r.levels[source], 0);
    // L[1]
    ASSERT_EQ((r.levels[TransformedVertex{0, true}]), 1);
    ASSERT_EQ((r.levels[TransformedVertex{3, true}]), 1);
    // L[2]
    ASSERT_EQ((r.levels[TransformedVertex{0, false}]), 2);
    ASSERT_EQ((r.levels[TransformedVertex{3, false}]), 2);
    // L[3]
    ASSERT_EQ((r.levels[TransformedVertex{1, true}]), 3);
    ASSERT_EQ((r.levels[TransformedVertex{4, true}]), 3);
    // L[4]
    ASSERT_EQ((r.levels[TransformedVertex{1, false}]), 4);
    ASSERT_EQ((r.levels[TransformedVertex{4, false}]), 4);
    // L[5]
    ASSERT_EQ((r.levels[TransformedVertex{2, true}]), 5);
    // L[6]
    ASSERT_EQ((r.levels[TransformedVertex{2, false}]), 6);
    // No level for t
    ASSERT_TRUE(r.levels.find(sink) == r.levels.end());
    ASSERT_EQ(r.distance, -1);
}

TEST(TestFindLevels, TestUnreachable) {
    // Simple path, will be unreachable because of blocking path
    auto disks = std::vector<Disk<int>>{
            {{1, 0}, 1},
            {{3, 0}, 1},
    };
    int left_border = 0;
    int right_border = 5;

    const auto paths = std::vector<Path>{
            {
                    Edge(source, {0, true}),
                    Edge({0, true}, {0, false}),
                    Edge({0, false}, {1, true}),
                    Edge({1, true}, {1, false}),
                    Edge({1, false}, sink),
            }
    };

    auto r = find_levels<int, Trivial>(paths, disks, left_border, right_border);
    ASSERT_FALSE(r.reachable);

    // Expect only level for s
    ASSERT_EQ(r.levels.size(), 1);
    ASSERT_EQ(r.levels[source], 0);
    ASSERT_EQ(r.distance, -1);

    // Two possible paths merge into one, which is blocked
    disks = std::vector<Disk<int>>{
            {{2, 0}, 3},
            {{5, 4}, 3},
            {{2, 8}, 3},
    };
    left_border = 0;
    right_border = 7;

    r = find_levels<int, Trivial>(paths, disks, left_border, right_border);
    ASSERT_FALSE(r.reachable);

    // Expect levels for vertex s and 5 for one reachable disks
    ASSERT_EQ(r.levels.size(), 6);
    // L[0]
    ASSERT_EQ(r.levels[source], 0);
    // L[1]
    ASSERT_EQ((r.levels[TransformedVertex{2, true}]), 1);
    // L[2]
    ASSERT_EQ((r.levels[TransformedVertex{2, false}]), 2);
    // L[3] - inbound vertex of disk 1
    ASSERT_EQ((r.levels[TransformedVertex{1, true}]), 3);
    // L[4] - outbound vertex of disk 0 (go backwards on path)
    ASSERT_EQ((r.levels[TransformedVertex{0, false}]), 4);
    // L[5] - inbound vertex of disk 0
    ASSERT_EQ((r.levels[TransformedVertex{0, true}]), 5);
    // No level for t
    ASSERT_TRUE(r.levels.find(sink) == r.levels.end());
    ASSERT_EQ(r.distance, -1);
}

TEST(TestFindLevels, TestReachable) {
    // Two possible ways to go, one is blocked
    auto disks = std::vector<Disk<int>>{
            {{1, 0}, 1},
            {{3, 0}, 1},
            {{1, 5}, 1},
            {{3, 5}, 1},
    };
    int left_border = 0;
    int right_border = 4;

    auto paths = std::vector<Path>{
            {
                    Edge(source, {0, true}),
                    Edge({0, true}, {0, false}),
                    Edge({0, false}, {1, true}),
                    Edge({1, true}, {1, false}),
                    Edge({1, false}, sink),
            }
    };

    auto r = find_levels<int, Trivial>(paths, disks, left_border, right_border);
    ASSERT_TRUE(r.reachable);

    // Expect levels for vertices s, t and 4 for reachable disks
    ASSERT_EQ(r.levels.size(), 6);

    // L[0]
    ASSERT_EQ(r.levels[source], 0);
    // L[1]
    ASSERT_EQ((r.levels[TransformedVertex{2, true}]), 1);
    // L[2]
    ASSERT_EQ((r.levels[TransformedVertex{2, false}]), 2);
    // L[3]
    ASSERT_EQ((r.levels[TransformedVertex{3, true}]), 3);
    // L[4]
    ASSERT_EQ((r.levels[TransformedVertex{3, false}]), 4);
    // L[5]
    ASSERT_EQ(r.levels[sink], 5);
    ASSERT_EQ(r.distance, 5);

    // Possible to go back on some edges (backwards on path)
    disks = std::vector<Disk<int>>{
            {{0, 0}, 1},
            {{0, 2}, 1},
            {{1, 1}, 1},
            {{1, 3}, 1},
            {{2, 2}, 1},
            {{2, 4}, 1},
    };
    left_border = -1;
    right_border = 3;

    paths = std::vector<Path>{
            {
                    Edge(source, {1, true}),
                    Edge({1, true}, {1, false}),
                    Edge({1, false}, {2, true}),
                    Edge({2, true}, {2, false}),
                    Edge({2, false}, {4, true}),
                    Edge({4, true}, {4, false}),
                    Edge({4, false}, sink),
            }
    };

    r = find_levels<int, Trivial>(paths, disks, left_border, right_border);
    ASSERT_TRUE(r.reachable);

    // Expect levels for vertices s, t and a lot for reachable disks
    ASSERT_EQ(r.levels.size(), 13);

    // L[0]
    ASSERT_EQ(r.levels[source], 0);
    // L[1]
    ASSERT_EQ((r.levels[TransformedVertex{0, true}]), 1);
    // L[2]
    ASSERT_EQ((r.levels[TransformedVertex{0, false}]), 2);
    // L[3]
    ASSERT_EQ((r.levels[TransformedVertex{2, true}]), 3);
    ASSERT_EQ((r.levels[TransformedVertex{1, true}]), 3);
    // L[4] - go back do outbound vertex of disk 1
    ASSERT_EQ((r.levels[TransformedVertex{1, false}]), 4);
    // L[5] - forward to disk 3. Can also go to disk 4
    ASSERT_EQ((r.levels[TransformedVertex{3, true}]), 5);
    ASSERT_EQ((r.levels[TransformedVertex{4, true}]), 5);
    // L[6]
    ASSERT_EQ((r.levels[TransformedVertex{3, false}]), 6);
    ASSERT_EQ((r.levels[TransformedVertex{2, false}]), 6);
    // L[7]
    ASSERT_EQ((r.levels[TransformedVertex{5, true}]), 7);
    // L[8]
    ASSERT_EQ((r.levels[TransformedVertex{5, false}]), 8);
    // L[9]
    ASSERT_EQ(r.levels[sink], 9);
    ASSERT_EQ(r.distance, 9);
}

TEST(TestFindLevels, TestLongerPathDropped) {
    // Check if other levels are dropped when we get to sink
    // Two possible ways to go, one is blocked
    auto disks = std::vector<Disk<int>>{
            // 2 disk path
            {{1, 10}, 1},
            {{3, 10}, 1},
            // 1 disk path, but will be blocked
            {{3, 0},  3},
            // 1 disk path
            {{3, 20}, 3},
    };
    int left_border = 0;
    int right_border = 4;

    auto paths = std::vector<Path>{
            {
                    Edge(source, {2, true}),
                    Edge({2, true}, {2, false}),
                    Edge({2, false}, sink),
            }
    };

    auto r = find_levels<int, Trivial>(paths, disks, left_border, right_border);
    ASSERT_TRUE(r.reachable);

    // Expect levels for vertices s, t and 4 for reachable disks (more than 4, but only 4 are reachable before reaching sink)
    ASSERT_EQ(r.levels.size(), 6);

    // L[0]
    ASSERT_EQ(r.levels[source], 0);
    // L[1]
    ASSERT_EQ((r.levels[TransformedVertex{0, true}]), 1);
    ASSERT_EQ((r.levels[TransformedVertex{3, true}]), 1);
    // L[2]
    ASSERT_EQ((r.levels[TransformedVertex{0, false}]), 2);
    ASSERT_EQ((r.levels[TransformedVertex{3, false}]), 2);
    // L[3] - reached sink
    ASSERT_EQ(r.levels[sink], 3);
    ASSERT_EQ(r.distance, 3);
}

// Test which helped me find a bug in the algorithm.
TEST(TestFindLevels, AdditionalTests) {
    const auto paths = std::vector<Path>{};

    // Trivial disk layout
    auto disks = std::vector<Disk<int>>{
            {{4, 10}, 1},
            {{0, 0},  1},
            {{2, 0},  1},
            {{2, 10}, 1},
            {{2, 2},  1},
            {{2, -2}, 1},
            {{0, 10}, 1},
            {{2, 12}, 1},
            {{2, 8},  1},
    };
    int left_border = -1;
    int right_border = 5;

    auto r = find_levels<int, Trivial>(paths, disks, left_border, right_border);
    ASSERT_TRUE(r.reachable);

    r = find_levels<int, Trivial>(paths, disks, left_border - 1, right_border);
    ASSERT_FALSE(r.reachable);

    r = find_levels<int, Trivial>(paths, disks, left_border, right_border + 1);
    ASSERT_FALSE(r.reachable);

}