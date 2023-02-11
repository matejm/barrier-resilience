#include "graph.hpp"

int graph_start_index(const Graph &) {
    return 0;
}

int graph_end_index(const Graph &graph) {
    return graph.size() - 1;
}

int graph_inbound_index(const Graph &, int disk_index) {
    return 2 * disk_index + 1;
}

int graph_outbound_index(const Graph &, int disk_index) {
    return 2 * disk_index + 2;
}

int graph_index_to_disk_index(const Graph &, int index) {
    return (index - 1) / 2;
}

void print_graph(const Graph &graph) {
    for (unsigned int i = 0; i < graph.size(); ++i) {
        std::cout << i << ": ";
        for (auto v: graph[i]) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
}
