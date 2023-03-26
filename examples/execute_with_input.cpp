#include <iostream>
#include <vector>
#include "utils/geometry_objects.hpp"
#include "barrier_resilience/config.hpp"
#include "barrier_resilience/barrier_resilience.hpp"

int main() {
    // Parse borders
    double left, right;
    std::cin >> left >> right;

    // Parse disks
    int n;
    std::cin >> n;
    std::vector<Disk<double>> disks;

    for (int i = 0; i < n; ++i) {
        double x, y, r;
        std::cin >> x >> y >> r;
        disks.emplace_back(Point<double>{x, y}, r);
    }

    // Compute barrier resilience
    const auto config = Config<double>::with_trivial_datastructure();
    auto to_remove = barrier_resilience_disks<double>(disks, left, right, config);

    // Print result
    std::cout << "Success " << to_remove.size() << " left=" << left << " right=" << right << " disks.size()="
              << disks.size() << " disks " << disks;
    for (auto i: to_remove) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}