#include <gtest/gtest.h>
#include "data_structure/kdtree.hpp"
#include <vector>

TEST(TestKDTree, TestQueryDisksOnly) {
    // KDTree sadly supports only disks with same radius
    const auto objects = std::vector<GeometryObject<int>>{
            Disk<int>{{0, 0}, 1},
            Disk<int>{{2, 2}, 1},
            Border<int>{10, false},
    };

    auto t = KDTree<int>();
    t.rebuild(objects);

    // Query objects and check if we always get intersecting disk
    ASSERT_EQ(t.intersecting(Disk<int>{{0, 0}, 1}), objects[0]);
    ASSERT_EQ(t.intersecting(Disk<int>{{-1, 0}, 1}), objects[0]);
    ASSERT_EQ(t.intersecting(Disk<int>{{2, 0}, 1}), objects[0]);
    ASSERT_EQ(t.intersecting(Disk<int>{{4, 2}, 1}), objects[1]);
    ASSERT_EQ(t.intersecting(Disk<int>{{5, 2}, 2}), objects[1]);
    ASSERT_EQ(t.intersecting(Disk<int>{{5, 2}, 2}), objects[1]);
    ASSERT_EQ(t.intersecting(Disk<int>{{-5, -5}, 8}), objects[0]);

    // Check intersection with border
    ASSERT_EQ(t.intersecting(Border<int>{9, false}), objects[2]);
    ASSERT_EQ(t.intersecting(Border<int>{100, false}), objects[2]);
    ASSERT_EQ(t.intersecting(Disk<int>{{9, 0}, 2}), objects[2]);
    ASSERT_EQ(t.intersecting(Disk<int>{{100, 0}, 2}), objects[2]);

    // Query objects that don't intersect any disk or border
    ASSERT_EQ(t.intersecting(Disk<int>{{-2, -2}, 1}), std::nullopt);
    ASSERT_EQ(t.intersecting(Disk<int>{{5, 5}, 1}), std::nullopt);
    ASSERT_EQ(t.intersecting(Disk<int>{{-100, 0}, 50}), std::nullopt);
}

TEST(TestKDTree, TestDelection) {
    const auto objects = std::vector<GeometryObject<int>>{
            Disk<int>{{0, 0}, 1},
            Disk<int>{{2, 2}, 1},
            Disk<int>{{4, 4}, 1},
            Disk<int>{{100, 100}, 1},
            Border<int>{-100, true},
    };

    auto t = KDTree<int>();
    t.rebuild(objects);

    // Delete objects and check if they are deleted
    t.delete_object(Disk<int>{{0, 0}, 1});
    ASSERT_EQ(t.intersecting(Disk<int>{{0, 0}, 1}), std::nullopt);
    ASSERT_EQ(t.intersecting(Disk<int>{{2, 1}, 1}), objects[1]);
    ASSERT_EQ(t.intersecting(Disk<int>{{4, 5}, 1}), objects[2]);
    ASSERT_EQ(t.intersecting(Disk<int>{{100, 101}, 1}), objects[3]);
    ASSERT_EQ(t.intersecting(Disk<int>{{-100, 0}, 1}), objects[4]);

    t.delete_object(Disk<int>{{2, 2}, 1});
    ASSERT_EQ(t.intersecting(Disk<int>{{2, 1}, 1}), std::nullopt);
    ASSERT_EQ(t.intersecting(Disk<int>{{4, 5}, 1}), objects[2]);
    ASSERT_EQ(t.intersecting(Disk<int>{{100, 101}, 1}), objects[3]);
    ASSERT_EQ(t.intersecting(Disk<int>{{-100, 0}, 1}), objects[4]);

    t.delete_object(Disk<int>{{4, 4}, 1});
    ASSERT_EQ(t.intersecting(Disk<int>{{4, 5}, 1}), std::nullopt);
    ASSERT_EQ(t.intersecting(Disk<int>{{100, 101}, 1}), objects[3]);
    ASSERT_EQ(t.intersecting(Disk<int>{{-100, 0}, 1}), objects[4]);

    t.delete_object(Disk<int>{{100, 100}, 1});
    ASSERT_EQ(t.intersecting(Disk<int>{{100, 101}, 1}), std::nullopt);
    ASSERT_EQ(t.intersecting(Disk<int>{{-100, 0}, 1}), objects[4]);

    t.delete_object(Border<int>{-100, true});
    ASSERT_EQ(t.intersecting(Disk<int>{{-100, 0}, 1}), std::nullopt);
}