#include "even_tarjan.hpp"

// Use vector of parents for each node to create BFS tree.
Graph create_bfs_tree(std::vector<int> parent) {
    // Create empty graph.
    Graph g(parent.size());
    // Add edges to the graph.
    for (int i = 0; i < parent.size(); i++) {
        if (parent[i] != -1) {
            // Add edge from parent to child.
            g[parent[i]].push_back(i);
        }
    }
    return g;
}

// Returns true if found a single path -> dfs must be terminated immediately.
bool dfs(const Graph &g, std::vector<bool> &visited, int start,
         int end, std::vector<int> &path, std::vector<std::vector<int>> &paths) {
    path.push_back(start);

    if (start == end) {
        // We found a path from start to end.

        // Copy path to paths.
        std::vector<int> path_copy(path);
        paths.push_back(path_copy);
        path.clear();

        // All vertices in path are already marked as visited, so we can terminate dfs.
        return true;
    }

    // Check all neighbours.
    for (int v: g[start]) {
        if (!visited[v]) {
            // Mark neighbour as visited, except for end node.
            if (v != end) {
                visited[v] = true;
            }
            // Recursively call dfs.
            bool found_path = dfs(g, visited, v, end, path, paths);

            if (found_path) {
                // We found a path, so we can terminate dfs.
                return true;
            }
        }
    }

    // Remove start from path.
    path.pop_back();
    return false;
}


// Perform DFS on BFS tree. When we find end node, we add the path to the set of paths.
//
// [ WARNING! ] Does not work if path from start to end has length 1. This happens because we don't mark start and end
// nodes as visited. This is not a problem, because in case of disk intersection, we will always have path from start to
// end with length at least 3 (start -> disk_in -> disk_out -> end).
//
// Returns set of paths from start to end. Set of paths is a blocking family.
std::vector<std::vector<int>> dfs_on_bfs_tree(const Graph &g, int start, int end) {
    std::vector<std::vector<int>> paths;
    std::vector<bool> visited(g.size(), false);

    // Run DFS on BFS tree.
    while (true) {
        // Start DFS from start node.
        std::vector<int> path;
        bool found_path = dfs(g, visited, start, end, path, paths);;

        if (!found_path) {
            // We didn't find any path, so we can terminate.
            break;
        }
    }

    return paths;
}

std::tuple<Graph, std::map<std::pair<int, int>, bool>, int> even_tarjan(const Graph &graph, int start, int end) {
    // We want to find max flow from start to end.
    // Create residual graph. Given graph has a special property: because graph was created from disks and expanded,
    // for each node in a graph holds the following property: graph is bipartite and graph has no cycles of length 2.
    // This means we won't change graph structure by adding back edge for each forward edge (and mark back edge as blocked).

    auto [residual_graph, blocked_edges] = prepare_residual_graph(graph);

    // Where we came from to each node.
    std::vector<int> parent(graph.size(), -1);
    // Visited vertices for BFS.
    std::vector<bool> visited(graph.size(), false);

    int phase = 0;
    int flow = 0;

    // Perform BFS until there is no path from start to end.
    while (bfs(residual_graph, blocked_edges, parent, visited, start, end)) {
        // Create BFS tree L from parent array.
        Graph bfs_tree = create_bfs_tree(parent);

        // Perform DFS on L to find paths from start to end.
        auto paths = dfs_on_bfs_tree(bfs_tree, start, end);

        // Update residual graph.
        for (auto path: paths) {
            for (int i = 0; i < path.size() - 1; i++) {
                // Block edge from path[i] to path[i + 1].
                blocked_edges[{path[i], path[i + 1]}] = true;
                // Unblock edge from path[i + 1] to path[i].
                blocked_edges[{path[i + 1], path[i]}] = false;
            }
        }

        // Reset visited and parent vectors.
        visited = std::vector<bool>(graph.size(), false);
        parent = std::vector<int>(graph.size(), -1);

        // Increase flow value.
        flow += paths.size();
        phase++;
    }

    return {residual_graph, blocked_edges, flow};
}

int even_tarjan_max_flow(const Graph &graph, int start, int end) {
    auto [residual_graph, blocked_edges, flow] = even_tarjan(graph, start, end);
    return flow;
}

// Returns vector of edges that are part of min cut.
std::vector<std::pair<int, int>> even_tarjan_min_cut(const Graph &graph, int start, int end) {
    auto [residual_graph, blocked_edges, flow] = even_tarjan(graph, start, end);
    return get_min_cut(graph, residual_graph, blocked_edges, start, end);
}