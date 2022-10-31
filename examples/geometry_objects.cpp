#include <vector>
#include <iostream>
#include "geometry_objects.hpp"

int main() {
    auto circles = std::vector<Circle<int>>{
        {{0, 0}, 1},
        {{1, 1}, 1},
        {{2, 2}, 2},
        {{3, 3}, 1},
        {{4, 4}, 1},
    };

    for (int i = 0; i < circles.size(); ++i) {
        for (int j = i + 1; j < circles.size(); ++j) {
            if (intersects(circles[i], circles[j])) {
                std::cout << "Circle " << i << " intersects circle " << j << std::endl;
            }
        }
    }


}