#include <gtest/gtest.h>
#include "utils/geometry_objects.hpp"
#include "with_graph_construction/graph.hpp"

TEST(TestGraph, TestGraphIndices) {
    const Graph g1 = Graph(6), g2 = Graph(146);

    // Index of start node
    ASSERT_EQ(graph_start_index(g1), 0);
    ASSERT_EQ(graph_start_index(g2), 0);

    // Index of end node
    ASSERT_EQ(graph_end_index(g1), 5);
    ASSERT_EQ(graph_end_index(g2), 145);

    // Index of inbound nodes
    ASSERT_EQ(graph_inbound_index(g1, 0), 1);
    ASSERT_EQ(graph_inbound_index(g1, 1), 3);
    ASSERT_EQ(graph_inbound_index(g2, 0), 1);
    ASSERT_EQ(graph_inbound_index(g2, 1), 3);
    ASSERT_EQ(graph_inbound_index(g2, 34), 69);
    ASSERT_EQ(graph_inbound_index(g2, 54), 109);

    // Index of outbound nodes
    ASSERT_EQ(graph_outbound_index(g1, 0), 2);
    ASSERT_EQ(graph_outbound_index(g1, 1), 4);
    ASSERT_EQ(graph_outbound_index(g2, 0), 2);
    ASSERT_EQ(graph_outbound_index(g2, 1), 4);
    ASSERT_EQ(graph_outbound_index(g2, 34), 70);
    ASSERT_EQ(graph_outbound_index(g2, 54), 110);

    // Reverse conversion
    ASSERT_EQ(graph_index_to_disk_index(g1, 1), 0);
    ASSERT_EQ(graph_index_to_disk_index(g1, 3), 1);
    ASSERT_EQ(graph_index_to_disk_index(g1, 2), 0);
    ASSERT_EQ(graph_index_to_disk_index(g1, 4), 1);
}

TEST(TestGraph, TestObjectsToGraph) {
    // Generic case
    const std::vector<GeometryObject<double>> objects = {
            Border<double>{-1, true},
            Disk<double>{Point<double>{0, 0}, 1},
            Disk<double>{Point<double>{0, 2}, 1},
            Disk<double>{Point<double>{2, 2}, 1},
            Disk<double>{Point<double>{4, 2}, 1},
            Border<double>{5, false}
    };

    Graph graph = objects_to_graph(objects);
    ASSERT_EQ(graph.size(), 6);
    ASSERT_EQ(graph, Graph({
                                   {1, 2},
                                   {0, 2},
                                   {0, 1, 3},
                                   {2, 4},
                                   {3, 5},
                                   {4}
                           }));

    // Special case - no disks
    const std::vector<GeometryObject<double>> objects2 = {
            Border<double>{-1, true},
            Border<double>{5, false}
    };
    graph = objects_to_graph(objects2);
    ASSERT_EQ(graph.size(), 2);
    ASSERT_EQ(graph, Graph({
                                   {},
                                   {}
                           }));
}

TEST(TestGraph, TestGenerateExpandedGraph) {
    // Minimal case
    const std::vector<Disk<double>> disks = {
            Disk<double>{Point<double>{0, 0}, 2},
    };
    Graph graph = generate_expanded_graph(disks, -1.0, 1.0);

    ASSERT_EQ(graph.size(), 4);
    ASSERT_EQ(graph, Graph({
                                   {1},
                                   {2},
                                   {3},
                                   {}
                           }));

    // Generic case
    const std::vector<Disk<double>> disks2 = {
            Disk<double>{Point<double>{1, 2}, 3},
            Disk<double>{Point<double>{1, -2}, 3},
            Disk<double>{Point<double>{3, 0}, 3},
    };
    graph = generate_expanded_graph(disks2, -1.0, 5.0);

    ASSERT_EQ(graph.size(), 8);
    ASSERT_EQ(graph, Graph({
                                   {1, 3},
                                   {2},
                                   {3, 5},
                                   {4},
                                   {1, 5},
                                   {6},
                                   // Nodes in any single line can be in any order, if tests fail, just change the order
                                   {7, 1, 3},
                                   {}
                           }));
}
