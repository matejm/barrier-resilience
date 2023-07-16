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

// Evaluate the problem for different
int evalutate_problem(const ProblemParams &params, const Config<int> &config) {
    std::vector<Disk<int>> disks;

    // Generate disks.
    for (int i = 0; i < params.number_of_disks; ++i) {
        disks.emplace_back(Disk<int>({rand() % (params.right - params.left) + params.left,
                                      rand() % (params.top - params.bottom) + params.bottom},
                                     params.radius));
    }

    // Solve the problem.
    return barrier_resilience_number_of_disks<int>(disks, params.left, params.right, config);
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


#endif //EXPERIMENTS_HELPERS_HPP
