#include <vector>
#include <iostream>
#include "geometry_objects.hpp"
#include "graph.hpp"

int main() {
    auto circles = std::vector<Circle<int>>{
        {{0, 0}, 1},
        {{1, 1}, 1},
        {{2, 2}, 2},
        {{3, 3}, 1},
        {{4, 4}, 1},
        {{5, 5}, 1}
    };

    auto graph = circles_to_graph(circles);
    for (int i = 0; i < graph.size(); ++i) {
        std::cout << "Circle " << i << " intersects with circles: ";
        for (int j = 0; j < graph[i].size(); ++j) {
            std::cout << graph[i][j] << " ";
        }
        std::cout << std::endl;
    }

}