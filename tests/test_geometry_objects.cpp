#include <gtest/gtest.h>
#include "utils/geometry_objects.hpp"

TEST(TestIntersection, DiskIntersectionNormal) {
    auto disks = std::vector<Disk<int>>{
            {{0, 0}, 1},
            {{1, 1}, 1},
            {{2, 2}, 2},
            {{3, 3}, 1},
    };

    ASSERT_TRUE(intersects(disks[0], disks[0]));
    ASSERT_TRUE(intersects(disks[0], disks[1]));
    ASSERT_TRUE(intersects(disks[0], disks[2]));
    ASSERT_FALSE(intersects(disks[0], disks[3]));
    ASSERT_TRUE(intersects(disks[1], disks[0]));
    ASSERT_TRUE(intersects(disks[1], disks[1]));
    ASSERT_TRUE(intersects(disks[1], disks[2]));
    ASSERT_FALSE(intersects(disks[1], disks[3]));
    ASSERT_TRUE(intersects(disks[2], disks[0]));
    ASSERT_TRUE(intersects(disks[2], disks[1]));
    ASSERT_TRUE(intersects(disks[2], disks[2]));
    ASSERT_TRUE(intersects(disks[2], disks[3]));
    ASSERT_FALSE(intersects(disks[3], disks[0]));
    ASSERT_FALSE(intersects(disks[3], disks[1]));
    ASSERT_TRUE(intersects(disks[3], disks[2]));
    ASSERT_TRUE(intersects(disks[3], disks[3]));
}

TEST(TestIntersection, DiskIntersectionEdgeCases) {
    // Touching disks
    ASSERT_TRUE(intersects(
            Disk<int>{{0, 0}, 1000000},
            Disk<int>{{1000001, 0}, 1}));
    ASSERT_FALSE(intersects(
            Disk<int>{{0, 0}, 999999},
            Disk<int>{{1000001, 0}, 1}));
    // One inside the other
    ASSERT_TRUE(intersects(
            Disk<int>{{0, 0}, 100},
            Disk<int>{{7, 0}, 1}));
    ASSERT_TRUE(intersects(
            Disk<int>{{7, 12}, 100},
            Disk<int>{{32, -2}, 8}));
    // Floating point
    ASSERT_TRUE(intersects(
            Disk<double>{{0.0, 0.0}, 1.0},
            Disk<double>{{0.0, 2.0}, 1.0}));
    ASSERT_FALSE(intersects(
            Disk<double>{{0.0, 0.0}, 0.999999999},
            Disk<double>{{0.0, 2.0}, 1.0}));
    ASSERT_TRUE(intersects(
            Disk<double>{{0.0, 0.0}, 1.414213562373095048801688724209698},
            Disk<double>{{2.0, 2.0}, 1.414213562373095048801688724209698}));
    ASSERT_FALSE(intersects(
            Disk<double>{{0.0, 0.0}, 1.414213562373095048801688724209698},
            Disk<double>{{2.0, 2.0}, 1.414213562373095048801688724209698 - 1e-10}));
}

TEST(TestIntersection, DiskBorderIntersection) {
    ASSERT_TRUE(intersects(
            Disk<int>{{0, 0}, 1},
            Border<int>{0, true}));
    ASSERT_TRUE(intersects(
            Disk<int>{{1, 0}, 1},
            Border<int>{0, true}));
    ASSERT_FALSE(intersects(
            Disk<double>{{1, 0}, 1 - 1e-10},
            Border<double>{0, true}));
    ASSERT_TRUE(intersects(
            Disk<double>{{1, 0}, 1 + 1e-10},
            Border<double>{0, true}));
    ASSERT_TRUE(intersects(
            Disk<int>{{100, 0}, 1},
            Border<int>{0, false}));
    ASSERT_FALSE(intersects(
            Disk<int>{{100, 0}, 1},
            Border<int>{0, true}));
    ASSERT_TRUE(intersects(
            Disk<int>{{100, 100}, 1},
            Border<int>{1, false}));
    ASSERT_FALSE(intersects(
            Disk<int>{{-100, 100}, 1},
            Border<int>{1, false}));
    ASSERT_TRUE(intersects(
            Disk<int>{{100, 100}, 100},
            Border<int>{0, true}));
    ASSERT_FALSE(intersects(
            Disk<int>{{100, 100}, 99},
            Border<int>{0, true}));
}
