#ifndef BARRIER_RESILIENCE_GRAPH_HPP
#define BARRIER_RESILIENCE_GRAPH_HPP

#include <vector>

// Graph represents a graph with nodes and edges.
// Nodes are represented by integers 0, 1, 2
// Can represent directed or undirected graphs.
template<class T>
using Graph = std::vector<std::vector<int>>;

template<class T>
Graph<T> objects_to_graph(const std::vector<GeometryObject<T>>& objects) {
    // graph[i] is the list of indices of disks that ith-disk intersects.
    auto graph = Graph<T>(objects.size());
    for (int i = 0; i < objects.size(); ++i) {
        for (int j = i + 1; j < objects.size(); ++j) {
            if (intersects(objects[i], objects[j])) {
                // Symmetric graph.
                graph[i].push_back(j);
                graph[j].push_back(i);
            }
        }
    }
    return graph;
}


#endif // BARRIER_RESILIENCE_GRAPH_HPP