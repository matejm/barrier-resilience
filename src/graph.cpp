#include "graph.hpp"

int graph_start_index(const Graph& graph) {
    return 0;
}

int graph_end_index(const Graph& graph) {
    return graph.size() - 1;
}

int graph_inbound_index(const Graph& graph, int disk_index) {
    return 2 * disk_index + 1;
}

int graph_outbound_index(const Graph& graph, int disk_index) {
    return 2 * disk_index + 2;
}

int graph_index_to_disk_index(const Graph& graph, int index) {
    return (index - 1) / 2;
}
