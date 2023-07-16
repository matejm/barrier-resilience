#ifndef DATA_STRUCTURE_KDTREE_HPP
#define DATA_STRUCTURE_KDTREE_HPP

#include <vector>
#include <optional>
#include <unordered_map>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Orthogonal_incremental_neighbor_search.h>
#include <CGAL/Search_traits_adapter.h>
#include <CGAL/Search_traits_2.h>
#include "utils/geometry_objects.hpp"
#include "data_structure/data_structure.hpp"

template<class T>
using Cartesian = CGAL::Simple_cartesian<T>;

template<class T>
using Point_2 = typename Cartesian<T>::Point_2;

template<class T>
using SearchTraits = CGAL::Search_traits_2<Cartesian<T>>;

// Add disk index to 2D point in a tree.
template<class T>
using Point_and_index = std::tuple<Point_2<T>, int>;

// Modified search traits that allows us to search on modified points.
template<class T>
using Traits = CGAL::Search_traits_adapter<
        Point_and_index<T>,
        CGAL::Nth_of_tuple_property_map<0, Point_and_index<T>>,
        SearchTraits<T>>;

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
using NN = CGAL::Orthogonal_incremental_neighbor_search<Traits<T>>;

template<class T>
class KDTree : public DataStructure<T> {
private:
    typename NN<T>::Tree tree;

    // Potentially left and right border.
    std::vector<Border<T>> borders;

    // Vector of all disks, used to check intersection with borders.
    std::vector<Disk<T>> disks;
    // Map marking if disk with given index was deleted.
    std::unordered_map<int, bool> deleted_disks;

    // Radius of all disks in the structure should be the same.
    T radius;

    Point_and_index<T> disk_to_point(const Disk<T> &disk) const {
        return std::make_tuple(
                Point_2<T>(disk.center.x, disk.center.y),
                disk.get_index()
        );
    }

    Disk<T> point_to_disk(const Point_and_index<T> &point) const {
        // Unpack point and index.
        const Point_2<T> p = std::get<0>(point);
        const int index = std::get<1>(point);

        auto disk = Disk<T>{{p.x(), p.y()}, radius};
        disk.unsafe_set_index(index);
        return disk;
    }

public:
    void rebuild(const std::vector<GeometryObject<T>> &objects) {
        tree.clear();
        disks = {};

        // Filter objects and add disks to the tree.
        for (const auto &o: objects) {
            if (is_disk(o)) {
                auto disk = std::get<Disk<T>>(o);

                // Insert into tree
                tree.insert(disk_to_point(disk));
                disks.push_back(disk);

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

        tree.build();
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

            // Check intersection with disks.
            // This is O(n) operation, but it's not a problem as we will check it only once.
            for (const auto disk: disks) {
                // Check only not deleted disks.
                if (!deleted_disks.contains(disk.get_index()) && intersects(border, disk)) {
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

        auto disk = std::get<Disk<T>>(object);

        // Query the tree for nearest neighbor.
        NN<T> nn(tree, std::get<0>(disk_to_point(disk)));

        auto it = nn.begin();
        if (it == nn.end()) {
            return {};
        }

        // Found a disk that intersects with the query disk.
        auto point = it->first;
        auto out_disk = point_to_disk(point);

        // We got the closest disk, but it might not intersect with the query disk.
        if (!intersects(object, static_cast<GeometryObject<T>>(out_disk))) {
            return {};
        }
        return {out_disk};
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
        deleted_disks[disk.get_index()] = true;
    }
};

#endif //DATA_STRUCTURE_KDTREE_HPP
