#include <gtest/gtest.h>
#include <vector>

#include "even_tarjan.hpp"

TEST(TestEvenTarjan, TestDfsOnBfsTree) {
    Graph t = {
            {1, 2},
            {3},
            {4},
            {4},
            {}
    };

    ASSERT_EQ(dfs_on_bfs_tree(t, 0, 4), std::vector<std::vector<int>>({
                                                                              {0, 1, 3, 4},
                                                                              {0, 2, 4}}));
    ASSERT_EQ(dfs_on_bfs_tree(t, 0, 3), std::vector<std::vector<int>>({{0, 1, 3}}));
    ASSERT_EQ(dfs_on_bfs_tree(t, 1, 4), std::vector<std::vector<int>>({{1, 3, 4}}));

    t = Graph({
                      {1,  5, 9},
                      {3},
                      {4},
                      {2},
                      {11},
                      {6,  7},
                      {8},
                      {11},
                      {},
                      {10, 11},
                      {},
                      {}
              });

    ASSERT_EQ(dfs_on_bfs_tree(t, 0, 11), std::vector<std::vector<int>>({
                                                                               {0, 1, 3, 2, 4, 11},
                                                                               {0, 5, 7, 11},
                                                                               {0, 9, 11},
                                                                       }));
    ASSERT_EQ(dfs_on_bfs_tree(t, 0, 10), std::vector<std::vector<int>>({
                                                                               {0, 9, 10},
                                                                       }));
    ASSERT_EQ(dfs_on_bfs_tree(t, 5, 11), std::vector<std::vector<int>>({
                                                                               {5, 7, 11},
                                                                       }));

    t = Graph({
                      {1, 3},
                      {2},
                      {5},
                      {4},
                      {7},
                      {6},
                      {7},
                      {}
              });

    ASSERT_EQ(dfs_on_bfs_tree(t, 0, 7), std::vector<std::vector<int>>({
                                                                              {0, 1, 2, 5, 6, 7},
                                                                              {0, 3, 4, 7},
                                                                      }));
}

TEST(TestEvenTarjan, TestCreateBfsTree) {
    ASSERT_EQ(create_bfs_tree({-1, 0, 0, 1, 1, 2, 2}), Graph({
                                                                     {1, 2},
                                                                     {3, 4},
                                                                     {5, 6},
                                                                     {},
                                                                     {},
                                                                     {},
                                                                     {}
                                                             }));

    ASSERT_EQ(create_bfs_tree({-1, 0, 0, 1, 1, 1, 2, 6}), Graph({{1, 2},
                                                                 {3, 4, 5},
                                                                 {6},
                                                                 {},
                                                                 {},
                                                                 {},
                                                                 {7},
                                                                 {}}));

    ASSERT_EQ(create_bfs_tree({-1, 0, 1, -1, 3, 3, 4, 5, 7, 7}), Graph({
                                                                               {1},
                                                                               {2},
                                                                               {},
                                                                               {4, 5},
                                                                               {6},
                                                                               {7},
                                                                               {},
                                                                               {8, 9},
                                                                               {},
                                                                               {}
                                                                       }));
}

TEST(TestEvenTarjan, TestEvenTarjan) {
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

    auto [residual_graph, blocked_edges, flow] = even_tarjan(g, 0, 7);
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

    auto [residual_graph2, blocked_edges2, flow2] = even_tarjan(g, 0, 1);
    ASSERT_EQ(flow2, 2);

    // Minimal example
    Graph g2 = {
            {1},
            {2},
            {3},
            {}
    };

    auto [residual_graph3, blocked_edges3, flow3] = even_tarjan(g2, 0, 3);
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

TEST(TestEvenTarjan, TestMaxFlow) {
    Graph g = {
            {1, 2},
            {3},
            {4},
            {5},
            {5},
            {}
    };

    ASSERT_EQ(even_tarjan_max_flow(g, 0, 5), 2);
    ASSERT_EQ(even_tarjan_max_flow(g, 0, 4), 1);
    ASSERT_EQ(even_tarjan_max_flow(g, 1, 5), 1);

    g = Graph({
                      {1},
                      {2},
                      {3},
                      {}
              });

    ASSERT_EQ(even_tarjan_max_flow(g, 0, 3), 1);

    g = Graph({
                      {1, 2},
                      {},
                      {},
                      {0, 1, 2}
              });
    ASSERT_EQ(even_tarjan_max_flow(g, 0, 3), 0);
}

TEST(TestEvenTarjan, TestMinCut) {
    Graph g = {
            {1, 2},
            {3},
            {4},
            {5},
            {5},
            {}
    };
    ASSERT_EQ(even_tarjan_min_cut(g, 0, 5), (std::vector<std::pair<int, int>>({{0, 1},
                                                                               {0, 2}})));
    ASSERT_EQ(even_tarjan_min_cut(g, 0, 4), (std::vector<std::pair<int, int>>({{0, 2}})));

    g = Graph({
                      {1, 2, 3},
                      {4},
                      {4},
                      {4},
                      {}
              });
    ASSERT_EQ(even_tarjan_min_cut(g, 0, 4), (std::vector<std::pair<int, int>>({{0, 1},
                                                                               {0, 2},
                                                                               {0, 3}})));

    g = Graph({
                      {1, 2, 3},
                      {4},
                      {4},
                      {5},
                      {5},
                      {}
              });
    ASSERT_EQ(even_tarjan_min_cut(g, 0, 5), (std::vector<std::pair<int, int>>({{0, 3},
                                                                               {4, 5}})));

}