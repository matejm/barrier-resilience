#ifndef WITH_GRAPH_CONSTRUCTION_EVEN_TARJAN_HPP
#define WITH_GRAPH_CONSTRUCTION_EVEN_TARJAN_HPP

#include <vector>
#include <map>
#include <queue>
#include "graph.hpp"
#include "ford_fulkerson.hpp"

// Max flow algorithm from Even and Tarjan.
// Returns residual graph, map of blocked edges and maximum flow.
// Idea of the algorithm:
// We perform following phases:
// - run BFS on residual graph and create BFS tree L
// - run DFS on L. When we find end node, we add the path to the set of paths. When DFS finishes, we have a blocking
//  family of paths. We update the residual graph.
// - if there is no path from start to end, we found max flow. Otherwise, we repeat the algorithm.
// There exists a proof that this algorithm will perform at most O(sqrt(V)) phases. This means that the algorithm
// will perform at most O(sqrt(V)) BFS and DFS, so the complexity is O(sqrt(V) * (V + E)).
std::tuple<Graph, std::map<std::pair<int, int>, bool>, int> even_tarjan(const Graph &graph, int start, int end);

int even_tarjan_max_flow(const Graph &graph, int start, int end);

std::vector<std::pair<int, int>> even_tarjan_min_cut(const Graph &graph, int start, int end);

std::vector<std::vector<int>> dfs_on_bfs_tree(const Graph &g, int start, int end);

Graph create_bfs_tree(std::vector<int> parent);

#endif //WITH_GRAPH_CONSTRUCTION_EVEN_TARJAN_HPP
