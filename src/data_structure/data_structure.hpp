#ifndef BARRIER_RESILIENCE_DATA_STRUCTURE_HPP
#define BARRIER_RESILIENCE_DATA_STRUCTURE_HPP

#include <vector>
#include <optional>
#include "utils/geometry_objects.hpp"

// Data structure from article, should have following operations:
// - creation from vector of disks
// - given a disk D (not necessarily from the structure), return a disk D' that intersects D (if any)
// - delete disk D (if it exists) from the structure

template<class T>
class DataStructure {
public:
    // Reconstruct data structure from vector of disks.
    virtual void rebuild(const std::vector<Disk<T>> &disks) = 0;

    // Given a disk D (not necessarily from the structure), return a disk D' that intersects D (if any).
    virtual std::optional<Disk<T>> intersecting_disk(const Disk<T> &disk) const = 0;

    // Delete disk D (if it exists) from the structure.
    virtual void delete_disk(const Disk<T> &disk) = 0;
};

#endif //BARRIER_RESILIENCE_DATA_STRUCTURE_HPP
