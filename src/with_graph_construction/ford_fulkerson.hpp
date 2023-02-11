#ifndef WITH_GRAPH_CONSTRUCTION_FORD_FULKERSON_HPP
#define WITH_GRAPH_CONSTRUCTION_FORD_FULKERSON_HPP

#include <vector>
#include <map>
#include <queue>
#include <tuple>
#include <iostream>
#include "graph.hpp"


// Returns true if there is a path form source to sink in residual graph.
// Function writes into parent vector, which is used to store the path.
// We do not need to clear parent vector, because all used indices are overwritten each time.
bool bfs(const Graph &g, const std::map<std::pair<int, int>, bool> &blocked_edges, std::vector<int> &parent,
         std::vector<bool> &visited, int start, int end);

// Reconstruction of path from start to end using parent vector.
std::vector<int> reconstruct_path(const std::vector<int> &parent, int start, int end);

std::pair<Graph, std::map<std::pair<int, int>, bool>> prepare_residual_graph(const Graph &graph);

std::vector<std::pair<int, int>> get_min_cut(const Graph &graph,
                                             const Graph &residual_graph,
                                             std::map<std::pair<int, int>, bool> &blocked_edges,
                                             int start,
                                             int end);

// Performs Ford-Fulkerson algorithm on graph from source start to sink end.
// Returns residual graph, map of blocked edges and maximum flow.
std::tuple<Graph, std::map<std::pair<int, int>, bool>, int> ford_fulkerson(const Graph &graph, int start, int end);

// Returns int, max flow from start to end in graph.
// Max flow equals to min cut, which is equal to minimum number of disks that need to be removed
// have a path from below to above.
int ford_fulkerson_max_flow(const Graph &graph, int start, int end);

// Returns vector of edges that are part of min cut.
std::vector<std::pair<int, int>> ford_fulkerson_min_cut(const Graph &graph, int start, int end);

#endif //WITH_GRAPH_CONSTRUCTION_FORD_FULKERSON_HPP
