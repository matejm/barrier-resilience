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
    std::vector<Disk<T>> disks;

public:
    // Create data structure from vector of disks.
    Trivial(const std::vector<Disk<T>> &disks_) : disks(disks_) {}

    void rebuild(const std::vector<Disk<T>> &disks_) {
        disks = disks_;
    }

    // Given a disk D (not necessarily from the structure), return a disk D' that intersects D (if any).
    // If none exists, return null pointer.
    std::optional<Disk<T>> intersecting_disk(const Disk<T> &disk) const {
        for (const auto &d: disks) {
            if (intersects(d, disk)) {
                return {d};
            }
        }
        return {};
    }

    // Delete disk D (if it exists) from the structure.
    void delete_disk(const Disk<T> &disk) {
        for (auto it = disks.begin(); it != disks.end(); ++it) {
            // We can use equality because we implemented custom equality operator for Disk.
            if (*it == disk) {
                disks.erase(it);
                return;
            }
        }
    }
};

#endif //BARRIER_RESILIENCE_TRIVIAL_HPP
