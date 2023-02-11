#include <gtest/gtest.h>
#include <algorithm>

#include "with_graph_construction/graph.hpp"
#include "with_graph_construction/ford_fulkerson.hpp"

TEST(TestFordFulkerson, TestBfs) {
    Graph g = {
            {1, 2},
            {0, 2},
            {0, 1, 3},
            {2, 4},
            {3, 5},
            {4},
            {7},
            {6},
    };
    std::map<std::pair<int, int>, bool> blocked_edges;
    for (unsigned int i = 0; i < g.size(); ++i) {
        for (unsigned int j: g[i]) {
            blocked_edges[{i, j}] = false;
        }
    }

    std::vector<int> parent(g.size(), -1);
    std::vector<bool> visited(g.size(), false);

    // No blocked edges
    ASSERT_TRUE(bfs(g, blocked_edges, parent, visited, 0, 5));
    ASSERT_EQ(parent, std::vector<int>({-1, 0, 0, 2, 3, 4, -1, -1}));
    ASSERT_EQ(visited, std::vector<bool>({true, true, true, true, true, true, false, false}));

    std::fill(visited.begin(), visited.end(), false);
    ASSERT_FALSE(bfs(g, blocked_edges, parent, visited, 0, 7));

    std::fill(visited.begin(), visited.end(), false);
    ASSERT_TRUE(bfs(g, blocked_edges, parent, visited, 4, 1));

    std::fill(visited.begin(), visited.end(), false);
    ASSERT_FALSE(bfs(g, blocked_edges, parent, visited, 6, 3));

    // Blocked edges
    blocked_edges[{0, 1}] = true;
    blocked_edges[{4, 5}] = true;
    std::fill(visited.begin(), visited.end(), false);
    std::fill(parent.begin(), parent.end(), -1);
    ASSERT_FALSE(bfs(g, blocked_edges, parent, visited, 0, 5));
    ASSERT_EQ(parent, std::vector<int>({-1, 2, 0, 2, 3, -1, -1, -1}));
    ASSERT_EQ(visited, std::vector<bool>({true, true, true, true, true, false, false, false}));

    blocked_edges[{0, 1}] = true;
    blocked_edges[{2, 3}] = true;
    blocked_edges[{2, 1}] = true;
    std::fill(visited.begin(), visited.end(), false);
    std::fill(parent.begin(), parent.end(), -1);
    ASSERT_TRUE(bfs(g, blocked_edges, parent, visited, 0, 2));
    ASSERT_EQ(parent, std::vector<int>({-1, -1, 0, -1, -1, -1, -1, -1}));
    ASSERT_EQ(visited, std::vector<bool>({true, false, true, false, false, false, false, false}));
}

TEST(TestFordFulkerson, TestReconstructPath) {
    std::vector<int> parent = {-1, 0, 0, 2, 3, 4, -1, -1};

    ASSERT_EQ(reconstruct_path(parent, 5), std::vector<int>({0, 2, 3, 4, 5}));
    ASSERT_EQ(reconstruct_path(parent, 4), std::vector<int>({0, 2, 3, 4}));
    ASSERT_EQ(reconstruct_path(parent, 0), std::vector<int>({0}));

    parent = {-1, -1, 0, -1, -1, -1, -1, -1};
    ASSERT_EQ(reconstruct_path(parent, 2), std::vector<int>({0, 2}));

    // No path - could be defined in some other way, but this is fine as we don't use it.
    ASSERT_EQ(reconstruct_path(parent, 3), std::vector<int>({3}));

    parent = {-1, 2, 0, 1, 5, 3, 5, 6, -1, -1, 7};
    ASSERT_EQ(reconstruct_path(parent, 10), std::vector<int>({0, 2, 1, 3, 5, 6, 7, 10}));
}

TEST(TestFordFulkerson, TestFordFulkerson) {
    Graph g = {
            {1, 3},
            {2},
            {3, 5},
            {4},
            {1, 5},
            {6},
            {1, 3, 7},
            {}
    };

    auto [residual_graph, blocked_edges, flow] = ford_fulkerson(g, 0, 7);
    ASSERT_EQ(flow, 1);

    // Order of edges in a single line is not important.
    // Sort edges to make it easier to compare
    for (auto &edges: residual_graph) {
        std::sort(edges.begin(), edges.end());
    }
    ASSERT_EQ(residual_graph, Graph({
                                            {1, 3},
                                            {0, 2, 4, 6},
                                            {1, 3, 5},
                                            {0, 2, 4, 6},
                                            {1, 3, 5},
                                            {2, 4, 6},
                                            {1, 3, 5, 7},
                                            {6}
                                    }));

    // There are multiple valid solutions, this is one of them.
    // If implementation is changed, this test might fail.
    ASSERT_EQ(blocked_edges, (std::map<std::pair<int, int>, bool>({
                                                                          {{0, 1}, true},
                                                                          {{0, 3}, false},
                                                                          {{1, 0}, false},
                                                                          {{1, 2}, true},
                                                                          {{1, 4}, true},
                                                                          {{1, 6}, true},
                                                                          {{2, 1}, false},
                                                                          {{2, 3}, false},
                                                                          {{2, 5}, true},
                                                                          {{3, 0}, true},
                                                                          {{3, 2}, true},
                                                                          {{3, 4}, false},
                                                                          {{3, 6}, true},
                                                                          {{4, 1}, false},
                                                                          {{4, 3}, true},
                                                                          {{4, 5}, false},
                                                                          {{5, 2}, false},
                                                                          {{5, 4}, true},
                                                                          {{5, 6}, true},
                                                                          {{6, 1}, false},
                                                                          {{6, 3}, false},
                                                                          {{6, 5}, false},
                                                                          {{6, 7}, true},
                                                                          {{7, 6}, false},
                                                                  })));

    auto [residual_graph2, blocked_edges2, flow2] = ford_fulkerson(g, 0, 1);
    ASSERT_EQ(flow2, 2);

    // Minimal example
    Graph g2 = {
            {1},
            {2},
            {3},
            {}
    };

    auto [residual_graph3, blocked_edges3, flow3] = ford_fulkerson(g2, 0, 3);
    ASSERT_EQ(flow3, 1);
    ASSERT_EQ(residual_graph3, Graph({
                                             {1},
                                             {0, 2},
                                             {1, 3},
                                             {2}
                                     }));
    ASSERT_EQ(blocked_edges3, (std::map<std::pair<int, int>, bool>({
                                                                           {{0, 1}, true},
                                                                           {{1, 0}, false},
                                                                           {{1, 2}, true},
                                                                           {{2, 1}, false},
                                                                           {{2, 3}, true},
                                                                           {{3, 2}, false},
                                                                   })));
}

TEST(TestFordFulkerson, TestMaxFlow) {
    Graph g = {
            {1, 3},
            {2},
            {3, 5},
            {4},
            {1, 5},
            {6},
            {1, 3, 7},
            {}
    };

    ASSERT_EQ(ford_fulkerson_max_flow(g, 0, 7), 1);
    ASSERT_EQ(ford_fulkerson_max_flow(g, 0, 5), 2);
    ASSERT_EQ(ford_fulkerson_max_flow(g, 0, 1), 2);
    ASSERT_EQ(ford_fulkerson_max_flow(g, 0, 2), 1);

    g[0].push_back(6);
    ASSERT_EQ(ford_fulkerson_max_flow(g, 0, 6), 2);
    ASSERT_EQ(ford_fulkerson_max_flow(g, 0, 7), 1);

    g[0].push_back(5);
    ASSERT_EQ(ford_fulkerson_max_flow(g, 0, 5), 3);
    ASSERT_EQ(ford_fulkerson_max_flow(g, 0, 6), 2);
}

TEST(TestFordFulkerson, TestMinCutSimple) {
    Graph g = {
            {1, 3},
            {2},
            {3, 5},
            {4},
            {1, 5},
            {6},
            {1, 3, 7},
            {}
    };

    // No other choice
    ASSERT_EQ(ford_fulkerson_min_cut(g, 0, 7), (std::vector<std::pair<int, int>>({{5, 6}})));
    ASSERT_EQ(ford_fulkerson_min_cut(g, 0, 2), (std::vector<std::pair<int, int>>({{1, 2}})));
    ASSERT_EQ(ford_fulkerson_min_cut(g, 0, 4), (std::vector<std::pair<int, int>>({{3, 4}})));

    // Multiple choices (but FF algorithm should always choose the same one)
    ASSERT_EQ(ford_fulkerson_min_cut(g, 0, 5), (std::vector<std::pair<int, int>>({{0, 1},
                                                                                  {0, 3}})));
    ASSERT_EQ(ford_fulkerson_min_cut(g, 0, 1), (std::vector<std::pair<int, int>>({{0, 1},
                                                                                  {0, 3}})));

    // Minimal example
    Graph g2 = {
            {1},
            {2},
            {3},
            {}
    };

    ASSERT_EQ(ford_fulkerson_min_cut(g2, 0, 3), (std::vector<std::pair<int, int>>({{0, 1}})));
}

TEST(TestFordFulkerson, TestMinCutGenerated) {
    // Two "fully" connected graphs on 100 vertices, connected by a single edge
    // (not fully connected, each individual graph has as many edges as fully connected graph would have,
    // but all edges are directed towards the larger indices)
    Graph g(200);

    for (int i = 0; i < 100; ++i) {
        for (int j = i + 1; j < 100; ++j) {
            g[i].push_back(j);
            g[i + 100].push_back(j + 100);
        }
    }

    // Add additional edge
    g[34].push_back(137);
    ASSERT_EQ(ford_fulkerson_min_cut(g, 0, 199), (std::vector<std::pair<int, int>>({{34, 137}})));
    // Test also max flow
    ASSERT_EQ(ford_fulkerson_max_flow(g, 0, 199), 1);

    // Add more edges
    g[23].push_back(145);
    g[23].push_back(146);
    ASSERT_EQ(ford_fulkerson_min_cut(g, 0, 199), (std::vector<std::pair<int, int>>({{23, 145},
                                                                                    {23, 146},
                                                                                    {34, 137}})));
    ASSERT_EQ(ford_fulkerson_max_flow(g, 0, 199), 3);

    // More edges
    g[42].push_back(133);
    g[69].push_back(111);
    ASSERT_EQ(ford_fulkerson_min_cut(g, 0, 199),
              (std::vector<std::pair<int, int>>({{23, 145},
                                                 {23, 146},
                                                 {34, 137},
                                                 {42, 133},
                                                 {69, 111}})));
    ASSERT_EQ(ford_fulkerson_max_flow(g, 0, 199), 5);

    // Not a single path to vertex 101
    ASSERT_EQ(ford_fulkerson_min_cut(g, 0, 101), (std::vector<std::pair<int, int>>({})));
    ASSERT_EQ(ford_fulkerson_max_flow(g, 0, 101), 0);

    // Add a path to vertex 101
    g[78].push_back(101);
    ASSERT_EQ(ford_fulkerson_min_cut(g, 0, 101), (std::vector<std::pair<int, int>>({{78, 101}})));
    ASSERT_EQ(ford_fulkerson_max_flow(g, 0, 101), 1);
}