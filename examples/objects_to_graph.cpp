#include <vector>
#include <iostream>
#include "utils/geometry_objects.hpp"
#include "with_graph_construction/graph.hpp"

int main() {
    // Objects to graph
    auto objects = std::vector<GeometryObject<int>>{
            Border<int>{0, true},
            Disk<int>{{0, 0}, 1},
            Disk<int>{{1, 1}, 1},
            Disk<int>{{2, 2}, 2},
            Disk<int>{{3, 3}, 1},
            Disk<int>{{4, 4}, 1},
            Disk<int>{{5, 5}, 1},
            Border<int>{5, false},
    };

    auto graph = objects_to_graph(static_cast<std::vector<GeometryObject<int>>>(objects));
    for (int i = 0; i < graph.size(); ++i) {
        std::cout << "Object " << i << " intersects with objects: ";
        for (int j = 0; j < graph[i].size(); ++j) {
            std::cout << graph[i][j] << " ";
        }
        std::cout << std::endl;
    }


    // Objects to expanded graph
    auto disks = std::vector<Disk<int>>{
            {{0, 0}, 1},
            {{1, 0}, 1},
    };

    graph = generate_expanded_graph(disks, 0, 2);

    for (int i = 0; i < graph.size(); ++i) {
        std::cout << i << ": ";
        for (int j = 0; j < graph[i].size(); ++j) {
            std::cout << graph[i][j] << " ";
        }
        std::cout << std::endl;
    }

}