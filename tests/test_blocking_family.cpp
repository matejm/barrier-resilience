#include <gtest/gtest.h>
#include <vector>

#include "data_structure/trivial.hpp"
#include "utils/geometry_objects.hpp"
#include "blocking_family.hpp"

TEST(TestBlockingFamily, TestEmptyPaths) {
    const auto blocked_edges = std::vector<Edge>{};

    // Simple path from source to sink
    auto disks = std::vector<Disk<int>>{
            {{0, 0}, 1},
            {{1, 1}, 1},
    };
    add_index(disks);
    int left_border = -1;
    int right_border = 2;

    // Expect single path of length 5
    std::vector<Path> family = find_blocking_family<int, Trivial>(blocked_edges, disks, left_border, right_border);
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
    add_index(disks);
    left_border = -1;
    right_border = 2;

    // Expect two paths of length 3
    family = find_blocking_family<int, Trivial>(blocked_edges, disks, left_border, right_border);
    // In case of different data structures, order of paths may be different.
    expected = {
            {{source, {1, true}}, {{1, true}, {1, false}}, {{1, false}, sink}},
            {{source, {0, true}}, {{0, true}, {0, false}}, {{0, false}, sink}},
    };
    EXPECT_EQ(family.size(), 2);
    EXPECT_EQ(family, expected);

    // Misleading paths
    disks = std::vector<Disk<int>>{
            {{0, 0},  1},
            {{2, 0},  1},
            {{2, 2},  1},
            {{2, -2}, 1},
            {{0, 10}, 1},
            {{2, 10}, 1},
            {{2, 12}, 1},
            {{2, 8},  1},
            {{4, 10}, 1},
            {{2, 14}, 1},
            {{2, 16}, 1},
    };
    add_index(disks);
    left_border = -1;
    right_border = 5;

    // Expect single path of length 7
    family = find_blocking_family<int, Trivial>(blocked_edges, disks, left_border, right_border);
    expected = {
            {{source, {4, true}}, {{4, true}, {4, false}}, {{4, false}, {5, true}}, {{5, true}, {5, false}},
             {{5, false}, {8, true}}, {{8, true}, {8, false}}, {{8, false}, sink}}
    };
    EXPECT_EQ(family, expected);

}

TEST(TestBlockingFamily, TestLongerPathsDropped) {
    auto blocked_edges = std::vector<Edge>{};

    // Shorter and longer path.
    auto disks = std::vector<Disk<int>>{
            {{0, 0},  1},
            {{2, 0},  1},
            {{2, 2},  1},
            {{2, 4},  1},
            {{4, 4},  1},
            {{2, 30}, 3},
    };
    add_index(disks);
    int left_border = -1;
    int right_border = 5;

    // Expect only shorter path to be found.
    // (we are searching in layered residual graph, so longer path is thrown away)
    std::vector<Path> family = find_blocking_family<int, Trivial>(blocked_edges, disks, left_border, right_border);
    // Order of paths may be different if algorithm changes.
    std::vector<Path> expected = {
            {{source, {5, true}}, {{5, true}, {5, false}}, {{5, false}, sink}},
    };
    EXPECT_EQ(family, expected);

    // Set shorter path as blocking path.
    blocked_edges = expected[0];

    // Expect longer path to be found.
    family = find_blocking_family<int, Trivial>(blocked_edges, disks, left_border, right_border);
    expected = {
            {{source, {0, true}}, {{0, true}, {0, false}}, {{0, false}, {1, true}}, {{1, true}, {1, false}},
             {{1, false}, {2, true}}, {{2, true}, {2, false}}, {{2, false}, {3, true}}, {{3, true}, {3, false}},
             {{3, false}, {4, true}}, {{4, true}, {4, false}}, {{4, false}, sink}},
    };
    EXPECT_EQ(family, expected);
}

TEST(TestBlockingFamily, TestPathsUnreachable) {
    auto blocked_edges = std::vector<Edge>{};

    // Shorter and longer path.
    auto disks = std::vector<Disk<int>>{
            {{0, 0}, 1},
            {{2, 0}, 1},
            {{2, 2}, 1},
            {{2, 4}, 1},
            {{4, 4}, 1},
    };
    add_index(disks);
    int left_border = -1;
    // Unreachable sink.
    int right_border = 6;

    // Expect no path to be found.
    std::vector<Path> family = find_blocking_family<int, Trivial>(blocked_edges, disks, left_border, right_border);
    std::vector<Path> expected = {};
    EXPECT_EQ(family, expected);

    // Now reachable sink.
    right_border = 5;

    // Path blocking all disks.
    blocked_edges = {{{source, {0, true}},
                      {{0, true}, {0, false}},
                      {{0, false}, {1, true}},
                      {{1, true}, {1, false}},
                      {{1, false}, {2, true}},
                      {{2, true}, {2, false}},
                      {{2, false}, {3, true}},
                      {{3, true}, {3, false}},
                      {{3, false}, {4, true}},
                      {{4, true}, {4, false}},
                      {{4, false}, sink}}};

    // Expect no path to be found.
    family = find_blocking_family<int, Trivial>(blocked_edges, disks, left_border, right_border);
    expected = {};
    EXPECT_EQ(family, expected);
}

TEST(TestBlockingFamily, TestTwoMergingPaths) {
    // Two paths merging into one and diverging again.
    auto disks = std::vector<Disk<int>>{
            // Path 1
            {{0, 0}, 1},
            {{2, 0}, 1},

            // Path 2
            {{0, 4}, 1},
            {{2, 4}, 1},

            // Common part
            {{2, 2}, 1},
            {{4, 2}, 1},
            {{6, 2}, 1},

            // Path 1
            {{6, 0}, 1},
            {{8, 0}, 1},

            // Path 2
            {{6, 4}, 1},
            {{8, 4}, 1},
    };
    add_index(disks);

    std::vector<Edge> no_blocked_edges = {};

    std::vector<Edge> first_path = {
            {source,     {0, true}},
            {{0, true},  {0, false}},
            {{0, false}, {1, true}},
            {{1, true},  {1, false}},
            {{1, false}, {4, true}},
            {{4, true},  {4, false}},
            {{4, false}, {5, true}},
            {{5, true},  {5, false}},
            {{5, false}, {6, true}},
            {{6, true},  {6, false}},
            {{6, false}, {7, true}},
            {{7, true},  {7, false}},
            {{7, false}, {8, true}},
            {{8, true},  {8, false}},
            {{8, false}, sink}
    };

    std::vector<Edge> second_path = {
            {source,     {2, true}},
            {{2, true},  {2, false}},
            {{2, false}, {3, true}},
            {{3, true},  {3, false}},
            {{3, false}, {4, true}},
            {{4, true},  {4, false}},
            {{4, false}, {5, true}},
            {{5, true},  {5, false}},
            {{5, false}, {6, true}},
            {{6, true},  {6, false}},
            // Here, path has multiple options where to go, default will be the first one.
            {{6, false}, {7, true}},
            {{7, true},  {7, false}},
            {{7, false}, {8, true}},
            {{8, true},  {8, false}},
            {{8, false}, sink}
    };

    // Starts as a second path, goes in different direction on diverging point.
    std::vector<Edge> third_path = {
            {source,      {2,  true}},
            {{2,  true},  {2,  false}},
            {{2,  false}, {3,  true}},
            {{3,  true},  {3,  false}},
            {{3,  false}, {4,  true}},
            {{4,  true},  {4,  false}},
            {{4,  false}, {5,  true}},
            {{5,  true},  {5,  false}},
            {{5,  false}, {6,  true}},
            {{6,  true},  {6,  false}},
            // Goes in different direction on diverging point.
            {{6,  false}, {9,  true}},
            {{9,  true},  {9,  false}},
            {{9,  false}, {10, true}},
            {{10, true},  {10, false}},
            {{10, false}, sink}
    };

    // Expect first path to be found (could be second path as well).
    auto family = find_blocking_family<int, Trivial>(no_blocked_edges, disks, -1, 9);
    EXPECT_EQ(family, (std::vector<std::vector<Edge>>{first_path}));

    // If first path is blocking, no path should be found.
    family = find_blocking_family<int, Trivial>(first_path, disks, -1, 9);
    EXPECT_EQ(family, (std::vector<std::vector<Edge>>{}));

    // If second path is blocking, no path should be found.
    family = find_blocking_family<int, Trivial>(second_path, disks, -1, 9);
    EXPECT_EQ(family, (std::vector<std::vector<Edge>>{}));

    // Block just a single edge from first path.
    // WARNING: Blocking a single edge is not a good idea, can produce unexpected results as algorithm doesn't check for
    // blocking path in every single step (we check only for previous edge and not for next edge, this is OK if on a path
    // but not ok if there is only a single edge).
    std::vector<Edge> blocking_paths = {
            {source, {0, true}}
    };

    // Expect second path to be found.
    family = find_blocking_family<int, Trivial>(blocking_paths, disks, -1, 9);
    EXPECT_EQ(family, (std::vector<std::vector<Edge>>{second_path}));

    // Block just a single edge from second path.
    blocking_paths = {{
                              {source, {2, true}}
                      }};

    // Expect first path to be found.
    family = find_blocking_family<int, Trivial>(blocking_paths, disks, -1, 9);
    EXPECT_EQ(family, (std::vector<std::vector<Edge>>{first_path}));

    // Block another edge from first path.
    blocking_paths = {
            {source,     {0, true}},
            {{7, false}, {8, true}}
    };
    // Expect third path to be found.
    family = find_blocking_family<int, Trivial>(blocking_paths, disks, -1, 9);
    EXPECT_EQ(family, (std::vector<std::vector<Edge>>{third_path}));

    // Block edge from merging point from one side
    blocking_paths = {
            {{1, false}, {4, true}},
            {{4, true},  {4, false}}
    };
    // Expect no path to be found.
    family = find_blocking_family<int, Trivial>(blocking_paths, disks, -1, 9);
    EXPECT_EQ(family, (std::vector<std::vector<Edge>>{}));
}
