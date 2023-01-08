#include <gtest/gtest.h>
#include "data_structure/trivial.hpp"
#include <vector>

TEST(TestTrivialDataStructure, TestQuery) {
    const auto disks = std::vector<Disk<int>>{
            {{0, 0}, 1},
            {{2, 2}, 1},
    };

    auto t = Trivial<int>(disks);

    // Query disks and check if we always get intersecting disk
    ASSERT_EQ(t.intersecting_disk({{0, 0}, 1}), disks[0]);
    ASSERT_EQ(t.intersecting_disk({{-1, 0}, 1}), disks[0]);
    ASSERT_EQ(t.intersecting_disk({{2, 0}, 1}), disks[0]);
    ASSERT_EQ(t.intersecting_disk({{4, 2}, 1}), disks[1]);
    ASSERT_EQ(t.intersecting_disk({{5, 2}, 2}), disks[1]);
    ASSERT_EQ(t.intersecting_disk({{5, 2}, 2}), disks[1]);
    ASSERT_EQ(t.intersecting_disk({{-5, -5}, 8}), disks[0]);

    // Query disks that don't intersect any disk
    ASSERT_EQ(t.intersecting_disk({{-2, -2}, 1}), std::nullopt);
    ASSERT_EQ(t.intersecting_disk({{5, 5}, 1}), std::nullopt);
    ASSERT_EQ(t.intersecting_disk({{100, 0}, 50}), std::nullopt);
}

TEST(TestTrivialDataStructure, TestDelection) {
    const auto disks = std::vector<Disk<int>>{
            {{0,   0},   1},
            {{2,   2},   1},
            {{4,   4},   1},
            {{100, 100}, 1},
    };

    auto t = Trivial<int>(disks);

    // Delete disks and check if they are deleted
    t.delete_disk({{0, 0}, 1});
    ASSERT_EQ(t.intersecting_disk({{0, 0}, 1}), std::nullopt);
    ASSERT_EQ(t.intersecting_disk({{2, 1}, 1}), disks[1]);
    ASSERT_EQ(t.intersecting_disk({{4, 5}, 1}), disks[2]);
    ASSERT_EQ(t.intersecting_disk({{100, 101}, 1}), disks[3]);

    t.delete_disk({{2, 2}, 1});
    ASSERT_EQ(t.intersecting_disk({{2, 1}, 1}), std::nullopt);
    ASSERT_EQ(t.intersecting_disk({{4, 5}, 1}), disks[2]);
    ASSERT_EQ(t.intersecting_disk({{100, 101}, 1}), disks[3]);

    t.delete_disk({{4, 4}, 1});
    ASSERT_EQ(t.intersecting_disk({{4, 5}, 1}), std::nullopt);
    ASSERT_EQ(t.intersecting_disk({{100, 101}, 1}), disks[3]);

    t.delete_disk({{100, 100}, 1});
    ASSERT_EQ(t.intersecting_disk({{100, 101}, 1}), std::nullopt);
}