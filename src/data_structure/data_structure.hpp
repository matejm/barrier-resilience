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
    virtual void rebuild(const std::vector<GeometryObject<T>> &objects) = 0;

    // Given a disk D (not necessarily from the structure), return structure that intersects D (if any).
    virtual std::optional<GeometryObject<T>> intersecting(const GeometryObject<T> &object) const = 0;

    // Delete object (if it exists) from the structure.
    virtual void delete_object(const GeometryObject<T> &o) = 0;
};

#endif //BARRIER_RESILIENCE_DATA_STRUCTURE_HPP
