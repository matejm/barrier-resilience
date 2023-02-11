#ifndef BARRIER_RESILIENCE_BARRIER_RESILIENCE_HPP
#define BARRIER_RESILIENCE_BARRIER_RESILIENCE_HPP

#include <vector>
#include <unordered_map>
#include "utils/geometry_objects.hpp"
#include "blocking_family.hpp"
#include "config.hpp"


template<class T>
int barrier_resilience_number_of_disks(std::vector<Disk<T>> &disks,
                                       const T &left_border_x,
                                       const T &right_border_x,
                                       const Config<T> &config);


#endif //BARRIER_RESILIENCE_BARRIER_RESILIENCE_HPP
