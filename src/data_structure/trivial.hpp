#ifndef BARRIER_RESILIENCE_TRIVIAL_HPP
#define BARRIER_RESILIENCE_TRIVIAL_HPP

#include <vector>
#include <optional>
#include "utils/geometry_objects.hpp"
#include "data_structure/data_structure.hpp"


// Trivial data structure, that stores all disks in a vector.
// In queries, it iterates over all disks and returns the first matching one.
template<class T>
class Trivial : public DataStructure<T> {
private:
    std::vector<GeometryObject<T>> objects;

public:
    void rebuild(const std::vector<GeometryObject<T>> &objects_) {
        objects = objects_;
    }

    // Given a disk D (not necessarily from the structure), return a disk D' that intersects D (if any).
    std::optional<GeometryObject<T>> intersecting(const GeometryObject<T> &object) const {
        for (const auto &o: objects) {
            if (intersects(o, object)) {
                return {o};
            }
        }
        return {};
    }

    // Delete object (if it exists) from the structure.
    void delete_object(const GeometryObject<T> &o) {
        for (auto it = objects.begin(); it != objects.end(); ++it) {
            // We can use equality because we implemented custom equality operator for geometry objects.
            if (*it == o) {
                objects.erase(it);
                return;
            }
        }
    }
};

#endif //BARRIER_RESILIENCE_TRIVIAL_HPP
