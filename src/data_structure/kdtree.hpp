#ifndef DATA_STRUCTURE_KDTREE_HPP
#define DATA_STRUCTURE_KDTREE_HPP

#include <vector>
#include <optional>
#include <unordered_map>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Orthogonal_incremental_neighbor_search.h>
#include <CGAL/Search_traits_2.h>
#include "utils/geometry_objects.hpp"
#include "data_structure/data_structure.hpp"

template<class T>
using Cartesian = CGAL::Simple_cartesian<T>;

// Possible strategies:
// - Orthogonal_k_neighbor_search
// - Orthogonal_incremental_neighbor_search
// - K_neighbor_search
// - Incremental_neighbor_search
// We use the 2nd one because:
// - it's incremental (allowing us to add and remove points).
// - it's orthogonal (great for Minkowski metric vs K_neighbor_search which is for Manhattan metric).
// https://doc.cgal.org/latest/Spatial_searching/index.html
template<class T>
using NN = CGAL::Orthogonal_incremental_neighbor_search<CGAL::Search_traits_2<Cartesian<T>>>;

// Simple point, used just to be hashed in unordered_map.
template<class T>
struct HPoint {
public:
    T x, y;
};

// Custom hash function for Point. T should have hash function.
template<class T>
class HPointHash {
public:
    size_t operator()(const HPoint<T> &p) const {
        return std::hash<T>()(p.x) ^ std::hash<T>()(p.y);
    }
};

template<typename T>
struct HPointEqual {
    bool operator()(const HPoint<T> &p1, const HPoint<T> &p2) const {
        return p1.x == p2.x && p1.y == p2.y;
    }
};

template<class T>
class KDTree : public DataStructure<T> {
private:
    typename NN<T>::Tree tree;

    // Potentially left and right border.
    std::vector<Border<T>> borders;

    // Points on each coordinate (to get disk index back).
    // There can be multiple disks with the same center.
    std::unordered_map<HPoint<T>, std::vector<int>, HPointHash<T>, HPointEqual<T>> disks_indices;

    // Radius of all disks in the structure should be the same.
    T radius;

    typename Cartesian<T>::Point_2 disk_to_point(const Disk<T> &disk) const {
        return typename Cartesian<T>::Point_2(disk.center.x, disk.center.y);
    }

    // Unsafe version of disk_to_point, that assumes that the object is a disk.
    typename Cartesian<T>::Point_2 disk_to_point(const GeometryObject<T> &disk) const {
        return disk_to_point(std::get<Disk<T>>(disk));
    }

    Disk<T> disk_from_point(const typename Cartesian<T>::Point_2 &point) const {
        return disk_from_point(HPoint<T>{point[0], point[1]});
    }

    Disk<T> disk_from_point(const HPoint<T> &point) const {
        // Create disk from point.
        auto disk = Disk<T>({point.x, point.y}, radius);

        // Get disk index from the map.
        // It is fine if we get the first one.
        auto disk_index = disks_indices.at(point)[0];
        disk.unsafe_set_index(disk_index);

        return disk;
    }

public:
    void rebuild(const std::vector<GeometryObject<T>> &objects_) {
        tree.clear();
        // Filter objects and add disks to the tree.
        for (const auto &o: objects_) {
            if (is_disk(o)) {
                auto disk = std::get<Disk<T>>(o);

                // Insert into tree
                tree.insert(disk_to_point(o));

                // Memorize disk index.
                auto point = HPoint<T>{disk.center.x, disk.center.y};
//                if (disks_indices.find(point) == disks_indices.end()) {
//                    disks_indices[point] = std::vector<int>();
//                }
//                disks_indices[point].push_back(disk.get_index());

                // Check if radius is the same for all disks.
                if (radius == 0) {
                    radius = disk.radius;
                } else {
                    assert("Radius of all disk should be the same" && radius == disk.radius);
                }
            } else {
                // This is a border.
                auto border = std::get<Border<T>>(o);
                borders.push_back(border);
            }
        }
    }

    // Given a disk D (not necessarily from the structure), return a disk D' that intersects D (if any).
    std::optional<GeometryObject<T>> intersecting(const GeometryObject<T> &object) const {
        if (!is_disk(object)) {
            // Intersection with border is actually not O(log n), but this will be fine for the algorithm
            // as intersection with borders will be checked only in first iteration.
            auto border = std::get<Border<T>>(object);

            // Check intersection with borders
            for (const auto &b: borders) {
                if (intersects(border, b)) {
                    return {b};
                }
            }

            // Check intersection with disks
            for (const auto &pair: disks_indices) {
                // Simply construct disk back from the point.
                auto disk = disk_from_point(pair.first);
                if (intersects(object, static_cast<GeometryObject<T>>(disk))) {
                    return {disk};
                }
            }

            return {};
        }

        // Check intersection with borders
        for (const auto &border: borders) {
            if (intersects(object, static_cast<GeometryObject<T>>(border))) {
                return {border};
            }
        }

        // Query the tree for nearest neighbor.
        NN<T> nn(tree, disk_to_point(object));

        auto it = nn.begin();
        if (it == nn.end()) {
            return {};
        }

        // Found a disk that intersects with the query disk.
        auto point = it->first;
        auto disk = disk_from_point(point);

        // We got the closest disk, but it might not intersect with the query disk.
        if (!intersects(object, static_cast<GeometryObject<T>>(disk))) {
            return {};
        }
        return {disk};
    }

    // Delete object (if it exists) from the structure.
    void delete_object(const GeometryObject<T> &o) {
        if (!is_disk(o)) {
            auto border = std::get<Border<T>>(o);
            // Delete border from the list of borders.
            for (auto it = borders.begin(); it != borders.end(); ++it) {
                if (*it == border) {
                    borders.erase(it);
                    return;
                }
            }

            return;
        }

        auto disk = std::get<Disk<T>>(o);

        // Remove point representing the disk from the tree.
        tree.remove(disk_to_point(disk));

        // Remove disk from the map.
        auto point = HPoint<T>{disk.center.x, disk.center.y};
        auto disk_index = disk.get_index();
        auto &disk_indices = disks_indices.at(point);

        // Remove the disk index from the vector.
        // Not optimal, but we don't expect many disks with the same center.
        for (auto it = disk_indices.begin(); it != disk_indices.end(); ++it) {
            if (*it == disk_index) {
                disk_indices.erase(it);
                break;
            }
        }
    }

};

#endif //DATA_STRUCTURE_KDTREE_HPP
