#include <gtest/gtest.h>
#include "data_structure/trivial.hpp"
#include "find_levels.hpp"
#include <vector>

TEST(TestFindLevels, TestEmptyPaths) {
    const auto paths = std::vector<Path>{};
    auto ds = Trivial<int>();

    // Trivial disk layout
    auto disks = std::vector<Disk<int>>{
            {{0, 0}, 1},
            {{1, 1}, 1},
    };
    int left_border = -1;
    int right_border = 2;

    auto r = find_levels(paths, disks, left_border, right_border, ds);
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

    // Another trivial disk layout
    disks = std::vector<Disk<int>>{
            {{0, 0}, 2},
            {{0, 3}, 2},
            {{2, 1}, 2},
    };
    left_border = -1;
    right_border = 3;

    r = find_levels(paths, disks, left_border, right_border, ds);
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

    r = find_levels(paths, disks, left_border, right_border, ds);
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

    // Another layout
    disks = std::vector<Disk<int>>{
            {{0, 0},  2},
            {{0, 3},  2},
            {{2, 10}, 3}
    };
    left_border = -1;
    right_border = 5;

    r = find_levels(paths, disks, left_border, right_border, ds);
    ASSERT_TRUE(r.reachable);
}

// TODO: add more tests
// - test unreachable
// - larger test cases
// - test if longer paths are ignored when sink is found