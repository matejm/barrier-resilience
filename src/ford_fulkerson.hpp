#ifndef BARRIER_RESILIENCE_FORD_FULKERSON_HPP
#define BARRIER_RESILIENCE_FORD_FULKERSON_HPP

#include <vector>
#include <map>
#include <queue>
#include <tuple>
#include "graph.hpp"


// Returns true if there is a path form source to sink in residual graph.
// Function writes into parent vector, which is used to store the path.
// We do not need to clear parent vector, because all used indices are overwritten each time.
bool bfs(const Graph& g, const std::map<std::pair<int, int>, bool>& blocked_edges, std::vector<int>& parent, std::vector<bool>& visited, int start, int end) {
    std::queue<int> q;
    q.push(start);
    visited[start] = true;
    parent[start] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (u == end) {
            return true;
        }

        for (int v : g[u]) {
            // If not visited and edge is not blocked.
            if (!visited[v] && !blocked_edges.at({u, v})) {
                visited[v] = true;
                q.push(v);
                // We came to v from u.
                parent[v] = u;
            }
        }
    }

    return false;
}

// Reconstruction of path from start to end using parent vector.
std::vector<int> reconstruct_path(const std::vector<int>& parent, int start, int end) {
    std::vector<int> path;
    // We start from end and go backwards.
    int current = end;
    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }
    // Reverse path, so that it starts from start and ends in end.
    std::reverse(path.begin(), path.end());
    return path;
}

void print_residual_graph(const Graph& g, int start, int end, const std::map<std::pair<int, int>, bool>& blocked_edges) {
    std::cout << "Residual graph:" << std::endl;
    for (int i = 0; i < g.size(); i++) {
        if (i == start) {
            std::cout << "Start: ";
        } else if (i == end) {
            std::cout << "End: ";
        } else {
            std::cout << i << " (disk_id: " << (i - 1) / 2 << ", inbound_edge: " << (i % 2 == 0) << "): ";
        }

        for (int j : g[i]) {
            if (j != end) {
                std::cout << j << " (disk_id: " << (j - 1) / 2 << ", blocked: " << blocked_edges.at({i, j}) << "), ";
            } else {
                std::cout << j << " (sink), ";
            }
        }
        std::cout << std::endl;
    }
}

// Performs Ford-Fulkerson algorithm on graph from source start to sink end.
// Returns residual graph, map of blocked edges and maximum flow.
std::tuple<Graph, std::map<std::pair<int, int>, bool>, int> ford_fulkerson(const Graph& graph, int start, int end) {
    // Create new graph which will be used as residual graph, but a little different.
    // If there is an edge from i to j in original graph, then there is an edge from i to j and from j to i in residual graph.
    // Map blocked_edges is used to keep track of blocked edges (at start, all reversed edges are blocked, when we find a path
    // from start to end, we unblock all reverse edges on that path and block all forward edges on that path).
    Graph residual_graph = std::vector<std::vector<int>>(graph.size());
    std::map<std::pair<int, int>, bool> blocked_edges;

    // Add edges to residual graph.
    for (int u = 0; u < graph.size(); u++) {
        for (int v: graph[u]) {
            // Forward edge - unblocked.
            residual_graph[u].push_back(v);
            blocked_edges[std::pair<int, int>(u, v)] = false;
            // Reverse edge - blocked.
            residual_graph[v].push_back(u);
            blocked_edges[std::pair<int, int>(v, u)] = true;
        }
    }

    // Where we came from to each node.
    std::vector<int> parent(graph.size(), -1);

    int flow = 0;
    std::vector<bool> visited(graph.size(), false);

    // While there is a path from start to end in residual graph.
    while (bfs(residual_graph, blocked_edges, parent, visited, start, end)) {
        // Do not forget to reset visited vector.
        std::fill(visited.begin(), visited.end(), false);

        // Reconstruct path from start to end.
        auto path = reconstruct_path(parent, start, end);

        // Unblock all reverse edges on path and block all forward edges on path.
        for (int i = 0; i < path.size() - 1; i++) {
            int u = path[i];
            int v = path[i + 1];
            blocked_edges[{u, v}] = true;
            blocked_edges[{v, u}] = false;
        }

        // Increase flow by 1.
        flow++;
    }

    // Once finished, there is no path from start to end in residual graph.
    // In each iteration of while loop, we found a path from start to end and increased flow by exactly 1.
    // So, max flow is equal to number of iterations of while loop.
    return {
        residual_graph,
        blocked_edges,
        flow
    };
}

// Returns int, max flow from start to end in graph.
// Max flow equals to min cut, which is equal to minimum number of disks that need to be removed
// have a path from below to above.
int ford_fulkerson_max_flow(const Graph& graph, int start, int end) {
    auto [residual_graph, blocked_edges, flow] = ford_fulkerson(graph, start, end);
    return flow;
}

// Returns vector of edges that are part of min cut.
std::vector<std::pair<int, int>> ford_fulkerson_min_cut(const Graph& graph, int start, int end) {
    auto [residual_graph, blocked_edges, flow] = ford_fulkerson(graph, start, end);

    // Perform BFS on residual graph once again.
    std::vector<bool> visited(residual_graph.size(), false);
    // We actually don't need parent vector.
    std::vector<int> parent(residual_graph.size(), -1);

    // Perform BFS on residual graph.
    bfs(residual_graph, blocked_edges, parent, visited, start, end);

    std::vector<std::pair<int, int>> min_cut;

    // Edges that are part of min cut are edges where
    // - starting node is visited
    // - ending node is not visited
    // - edge is blocked (should be anyway)
    // - edge is from original graph (not from residual graph)
    for (int i = 0; i < graph.size(); i++) {
        if (!visited[i]) {
            // Skip unvisited nodes
            continue;
        }
        // Look over original graph (not residual graph).
        for (int j : graph[i]) {
            if (!visited[j] && blocked_edges[{i, j}]) {
                min_cut.push_back({i, j});
            }
        }
    }

    return min_cut;
}

#endif //BARRIER_RESILIENCE_FORD_FULKERSON_HPP
