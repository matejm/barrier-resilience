#include <gtest/gtest.h>
#include <vector>

#include "data_structure/trivial.hpp"
#include "utils/geometry_objects.hpp"
#include "blocking_family.hpp"


TEST(TestBlockingFamily, TestEmptyPaths) {
    const auto paths = std::vector<Path>{};

    // Simple path from source to sink
    auto disks = std::vector<Disk<int>>{
            {{0, 0}, 1},
            {{1, 1}, 1},
    };
    int left_border = -1;
    int right_border = 2;

    // Expect single path of length 5
    std::vector<Path> family = find_blocking_family<int, Trivial>(paths, disks, left_border, right_border);
    std::vector<Path> expected = {
            {{source, {0, true}}, {{0, true}, {0, false}}, {{0, false}, {1, true}}, {{1, true}, {1, false}},
             {{1, false}, sink}}
    };
    EXPECT_EQ(family, expected);

    // Two short paths from source to sink
    disks = std::vector<Disk<int>>{
            {{0, 0},  2},
            {{0, 10}, 2},
    };
    left_border = -1;
    right_border = 2;

    // Expect two paths of length 3
    family = find_blocking_family<int, Trivial>(paths, disks, left_border, right_border);
    // In case of different data structures, order of paths may be different.
    expected = {
            {{source, {1, true}}, {{1, true}, {1, false}}, {{1, false}, sink}},
            {{source, {0, true}}, {{0, true}, {0, false}}, {{0, false}, sink}},
    };
    EXPECT_EQ(family.size(), 2);
    EXPECT_EQ(family, expected);
}
