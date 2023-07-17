#ifndef EXPERIMENTS_HELPERS_HPP
#define EXPERIMENTS_HELPERS_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include "utils/geometry_objects.hpp"
#include "utils/visualize.hpp"
#include "barrier_resilience/barrier_resilience.hpp"
#include "barrier_resilience/config.hpp"
#include "with_graph_construction/graph_barrier_resilience.hpp"

void check_eq(int a, int b) {
    if (a == b) {
        return;
    }
    throw std::runtime_error("Expected " + std::to_string(a) + " but got " + std::to_string(b));
}

// simple class for measuring elapsed time
class Timer {
public:
    typedef std::chrono::high_resolution_clock Clock;

    void start() {
        epoch = Clock::now();
    }

    // time elapsed in nanoseconds
    Clock::duration time_elapsed_exact() const {
        return Clock::now() - epoch;
    }

    // time elapsed in seconds
    double time_elapsed() const {
        return std::chrono::duration_cast<std::chrono::microseconds>(time_elapsed_exact()).count() / 1000000.0;
    }


private:
    Clock::time_point epoch;
};


// Params of the barrier resilience problem to be solved.
struct ProblemParams {
    // Disk radius.
    int radius;

    // Left and right border. (limit where disks can be placed and where the left and right barriers are).
    int left;
    int right;

    // Bottom and top border (limit where disks can be placed).
    int bottom;
    int top;

    // Number of disks to be placed.
    int number_of_disks;
};


std::vector<Disk<int>> generate_disks(const ProblemParams &params) {
    std::vector<Disk<int>> disks;

    // Generate disks.
    for (int i = 0; i < params.number_of_disks; ++i) {
        disks.emplace_back(Disk<int>({rand() % (params.right - params.left) + params.left,
                                      rand() % (params.top - params.bottom) + params.bottom},
                                     params.radius));
    }

    return disks;
}


// Evaluate the problem for different
int evalutate_problem(const ProblemParams &params, const Config<int> &config) {
    auto disks = generate_disks(params);

    // Solve the problem.
    return barrier_resilience_number_of_disks<int>(disks, params.left, params.right, config);
}

int evalutate_problem(const ProblemParams &params, const Algorithm &algorithm) {
    auto disks = generate_disks(params);

    for (auto &disk: disks) {
        std::cout << disk << std::endl;
    }

    // Solve the problem.
    return graph_barrier_resilience_number_of_disks(disks, params.left, params.right, algorithm);
}

std::vector<double> compare_algorithms(const ProblemParams &params, const std::vector<Config<int>> &configs,
                                       const std::vector<Algorithm> &algorithms = {},
                                       const std::vector<Disk<int>> &disks_ = {}) {

    auto disks = std::vector<Disk<int>>(disks_);

    if (disks.empty()) {
        disks = generate_disks(params);
    }

    std::vector<int> results;
    std::vector<double> times;
    auto timer = Timer();

    for (const auto &config: configs) {
        timer.start();

        // Solve the problem.
        int result = barrier_resilience_number_of_disks(disks, params.left, params.right, config);

        times.push_back(timer.time_elapsed());
        results.push_back(result);
    }

    for (const auto &algorithm: algorithms) {
        timer.start();

        // Solve the problem.
        int result = graph_barrier_resilience_number_of_disks(disks, params.left, params.right, algorithm);

        times.push_back(timer.time_elapsed());
        results.push_back(result);
    }

    // Check that all results are the same.
    for (int i = 1; i < results.size(); ++i) {
        check_eq(results[i - 1], results[i]);
    }

    return times;
}


#endif //EXPERIMENTS_HELPERS_HPP
