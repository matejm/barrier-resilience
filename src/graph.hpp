#ifndef BARRIER_RESILIENCE_GRAPH_HPP
#define BARRIER_RESILIENCE_GRAPH_HPP

#include <vector>
#include "geometry_objects.hpp"

// Graph represents a graph with nodes and edges.
// Nodes are represented by integers 0, 1, 2
// Can represent directed or undirected graphs.
using Graph = std::vector<std::vector<int>>;

// Helper functions for expanded graph.
int graph_start_index(const Graph& graph);

int graph_end_index(const Graph& graph);

int graph_inbound_index(const Graph& graph, int disk_index);

int graph_outbound_index(const Graph& graph, int disk_index);

int graph_index_to_disk_index(const Graph& graph, int index);

// Creates "expanded" graph from vector of disks.
// Graph looks like this: undirected graph, represented by adjacency list,
// graph[i] is a vector of indices of objects that have at least one point in common with object i.
//
// To get "expanded" graph, each node representing a disk is expanded into 2 nodes:
// - we look at graph as a directed graph,
// - we add all edges pointing to node i in upper graph to point to node 2*i + 1,
// - we add all edges pointing from node i in upper graph to point from node 2*i + 2,
// - we connect node 2*i + 1 to node 2*i + 2 with a directed edge.
// We add connections from start (left border) to each node which intersects border (2*i + 1 for all i-s where i-th disk
// intersects border) and connection to end (right border) from every node which intersects border (2*i + 2 for all i-s
// where i-th disk intersects right border).
template<class T>
Graph generate_expanded_graph(const std::vector<Disk<T>>& disks, T left_border_x, T right_border_x) {
    const Border<T>
            left_border = Border<T>{left_border_x, true},
            right_border = Border<T>{right_border_x, false};

    // Index 0 = start
    // 2 * i + 1 = inbound vertex for each disk
    // 2 * i + 2 = out vertex for each disk
    // Last index = end
    auto graph = Graph(1 + 2 * disks.size() + 1);

    int start_index = graph_start_index(graph);
    int end_index = graph_end_index(graph);

    for (int i = 0; i < disks.size(); ++i) {
        // All connections from start
        if (intersects(disks[i], left_border)) {
            graph[start_index].push_back(graph_inbound_index(graph, i));
        }

        // All connections to end
        if (intersects(disks[i], right_border)) {
            graph[graph_outbound_index(graph, i)].push_back(end_index);
        }

        // Connections 2*i + 1 -> 2*i + 2
        graph[graph_inbound_index(graph, i)].push_back(graph_outbound_index(graph, i));
    }

    // Add edges between disks.
    for (int i = 0; i < disks.size(); ++i) {
        for (int j = i + 1; j < disks.size(); ++j) {
            if (intersects(disks[i], disks[j])) {
                // Add connections from disks i and j
                graph[graph_outbound_index(graph, i)].push_back(graph_inbound_index(graph, j));
                graph[graph_outbound_index(graph, j)].push_back(graph_inbound_index(graph, i));
            }
        }
    }

    return graph;
}

template<class T>
Graph objects_to_graph(const std::vector<GeometryObject<T>>& objects) {
    // graph[i] is the list of indices of disks that ith-disk intersects.
    auto graph = Graph(objects.size());
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