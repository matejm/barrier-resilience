#ifndef BARRIER_RESILLIENCE_GRAPH_HPP
#define BARRIER_RESILLIENCE_GRAPH_HPP

#include <vector>

// Graph represents a graph with nodes and edges.
// Nodes are represented by integers 0, 1, 2
// Can represent directed or undirected graphs.
template<class T>
using Graph = std::vector<std::vector<int>>;

template<class T>
Graph<T> circles_to_graph(const std::vector<Circle<T>>& circles) {
    // graph[i] is the list of indices of circles that ith-circle intersects.
    auto graph = Graph<T>(circles.size());
    for (int i = 0; i < circles.size(); ++i) {
        for (int j = i + 1; j < circles.size(); ++j) {
            if (intersects(circles[i], circles[j])) {
                // Symmetric graph.
                graph[i].push_back(j);
                graph[j].push_back(i);
            }
        }
    }
    return graph;
}


#endif // BARRIER_RESILLIENCE_GRAPH_HPP