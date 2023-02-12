#ifndef BARRIER_RESILIENCE_BARRIER_RESILIENCE_HPP
#define BARRIER_RESILIENCE_BARRIER_RESILIENCE_HPP

#include <vector>
#include <unordered_map>
#include "utils/geometry_objects.hpp"
#include "blocking_family.hpp"
#include "config.hpp"

// Returns a minimum number of disks that need to be removed to be able to
// move from top to bottom without colliding with any of the remaining disks.
template<class T>
int barrier_resilience_number_of_disks(std::vector<Disk<T>> &disks,
                                       const T &left_border_x,
                                       const T &right_border_x,
                                       const Config<T> &config);

// Returns one possible solution for the barrier resilience problem.
// (i.e. a set of disks that need to be removed to be able to move from
// top to bottom without colliding with any of the remaining disks)
// Returns indices of specific disks which need to be removed.
template<class T>
std::vector<int> barrier_resilience_disks(std::vector<Disk<T>> &disks,
                                          const T &left_border_x,
                                          const T &right_border_x,
                                          const Config<T> &config);

#endif //BARRIER_RESILIENCE_BARRIER_RESILIENCE_HPP
