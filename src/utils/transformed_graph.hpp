#ifndef BARRIER_RESILIENCE_TRANSFORMED_GRAPH_HPP
#define BARRIER_RESILIENCE_TRANSFORMED_GRAPH_HPP

#include <vector>
#include <iostream>

// Vertex in transformed graph.
// (in original graph G, vertices are disks + s and t)
struct TransformedVertex {
public:
    // Index of disk in original graph.
    // -1 for s and t
    int disk_index;
    // Is this a inbound or outbound vertex?
    // - inbound: only one outgoing edge, to the outbound vertex
    // - outbound: only one incoming edge, from the inbound vertex
    bool inbound;

    // Equality operator.
    bool operator==(const TransformedVertex &other) const {
        return disk_index == other.disk_index && inbound == other.inbound;
    }

    friend std::ostream &operator<<(std::ostream &o, TransformedVertex const &v) {
        return o << "(" << v.disk_index << ", " << std::boolalpha << v.inbound << ")";
    };
};


// Source will be transformed vertex with disk_index = -1 and inbound = false.
// (has a lot of outgoing edges)
const TransformedVertex source = {-1, false};

// Sink will be transformed vertex with disk_index = -1 and inbound = true.
const TransformedVertex sink = {-1, true};

// Edge in G' (transformed graph).
// Can be either
// - from source to inbound vertex of disk i
// - from outbound vertex of disk i to sink
// - from outbound vertex of disk i to inbound vertex of disk j
// - from inbound vertex of disk i to outbound vertex of disk i
// This means that graph G' is a bipartite graph.
class Edge {
public:
    TransformedVertex from;
    TransformedVertex to;

    Edge(TransformedVertex from, TransformedVertex to) : from(from), to(to) {}

    static Edge from_source(TransformedVertex to) {
        return Edge(source, to);
    }

    static Edge to_sink(TransformedVertex from) {
        return Edge(from, sink);
    }

    // Is this edge internal edge of a disk?
    // (i.e. is it an edge between two inbound/outbound vertices of the same disk)
    bool is_internal_edge() const {
        return from.disk_index == to.disk_index;
    }

    // Equality operator.
    bool operator==(const Edge &other) const {
        return from == other.from && to == other.to;
    }

    friend std::ostream &operator<<(std::ostream &o, Edge const &e) {
        return o << e.from << " -> " << e.to;
    }
};


// Directed path from s to d in the graph G'.
// The path is represented as a sequence of edges from G'
using Path = std::vector<Edge>;

// Custom hash function for transformed vertices.
class TransformedVertexHash {
public:
    size_t operator()(const TransformedVertex &vertex) const {
        return std::hash<int>()(vertex.disk_index) ^ std::hash<bool>()(vertex.inbound);
    }
};

// Custom hash function for edges.
class EdgeHash {
public:
    size_t operator()(const Edge &edge) const {
        return TransformedVertexHash()(edge.from) ^ ~TransformedVertexHash()(edge.to);
    }
};


#endif //BARRIER_RESILIENCE_TRANSFORMED_GRAPH_HPP
