#ifndef UTILS_GEOMETRY_OBJECTS_HPP
#define UTILS_GEOMETRY_OBJECTS_HPP

#include <variant>
#include "utils.hpp"
#include "transformed_graph.hpp"

template<class T>
struct Point {
    T x;
    T y;

    // Equality operator.
    bool operator==(const Point<T> &other) const {
        return x == other.x && y == other.y;
    }
};

// Disk with center and radius
template<class T>
class Disk {
    // Helper value, used to identify disks in the data structure.
    int index;
public:
    Point<T> center;
    T radius;

    Disk(Point<T> center, T radius) : center(center), radius(radius) {
        // Set index to -1, will be set later if needed.
        index = -1;
    }

    int get_index() const {
        return index;
    }

    // Equality operator.
    bool operator==(const Disk<T> &other) const {
        return center == other.center && radius == other.radius;
    }

    // Convert disk to two vertices in a transformed graph G'.
    // (inbound and outbound vertex)
    std::pair<TransformedVertex, TransformedVertex> to_transformed_vertices() const {
        return {
                disk_to_transformed_vertex(this, true),
                disk_to_transformed_vertex(this, false),
        };
    }

    void unsafe_set_index(int index_) {
        index = index_;
    }

    // Friend - this method can set index of a disk.
    template<class S>
    friend void add_index_to_disks(std::vector<Disk<S>> &disks);

    friend std::ostream &operator<<(std::ostream &o, Disk const &disk) {
        return o << "Disk(" << disk.center.x << ", " << disk.center.y << ", r=" << disk.radius << ")";
    }
};

// Add indices to disks.
template<class T>
void add_index_to_disks(std::vector<Disk<T>> &disks) {
    for (unsigned int i = 0; i < disks.size(); ++i) {
        disks[i].index = i;
    }
}

// Left or right border - vertical segment with given x coordinate
template<class T>
struct Border {
    T x;
    // Is this left or right border?
    bool left;

    // Equality operator.
    bool operator==(const Border<T> &other) const {
        return x == other.x && left == other.left;
    }

    friend std::ostream &operator<<(std::ostream &o, Border const &border) {
        return o << "Border(" << border.x << ", " << (border.left ? "left" : "right") << ")";
    }
};

// Custom hash function for transformed vertices.
template<class T>
class BorderHash {
public:
    size_t operator()(const Border<T> &border) const {
        return std::hash<T>()(border.x) ^ std::hash<bool>()(border.left);
    }
};

// Geometry object is either a disk or a border.
template<class T>
using GeometryObject = std::variant<Disk<T>, Border<T>>;

template<class T>
bool is_disk(const GeometryObject<T> &object) {
    return std::holds_alternative<Disk<T>>(object);
}

template<class T>
bool is_border(const GeometryObject<T> &object) {
    return std::holds_alternative<Border<T>>(object);
}

// Intersects returns true if the two disks have a non-empty intersection.
// (if one disk is contained in the other, they are considered to intersect)
template<class T>
bool intersects(const Disk<T> &d1, const Disk<T> &d2) {
    T dx = d1.center.x - d2.center.x;
    T dy = d1.center.y - d2.center.y;
    // Keep distance squared to avoid sqrt for better numerical stability.
    T d_squared = dx * dx + dy * dy;
    // Sum of radii squared.
    T r_squared = (d1.radius + d2.radius) * (d1.radius + d2.radius);
    return d_squared <= r_squared;
}

// Intersection of border and disk.
template<class T>
bool intersects(const Disk<T> &d, const Border<T> &b) {
    if (b.left) {
        // Check if disk intersects left border.
        if (d.center.x <= b.x) {
            // Center of the disk is to the left of the border.
            return true;
        }
        // Distance from the center of the disk to the border is less than the radius.
        return d.center.x - d.radius <= b.x;
    }
    // Check if disk intersects right border.
    if (d.center.x >= b.x) {
        // Center of the disk is to the right of the border.
        return true;
    }
    // Distance from the center of the disk to the border is less than the radius.
    return b.x <= d.center.x + d.radius;
}

template<class T>
bool intersects(const Border<T> &b, const Disk<T> &d) {
    return intersects(d, b);
}

template<class T>
bool intersects(const Border<T> &b1, const Border<T> &b2) {
    if (b1.left == b2.left) {
        // Both borders are left or both are right.
        return true;
    }
    // If left border is to the right of the right border, they do intersect.
    if (b1.left) {
        return b1.x >= b2.x;
    }
    return b2.x >= b1.x;
}

template<class T>
bool intersects(const GeometryObject<T> &g1, const GeometryObject<T> &g2) {
    return g1 | match{
            [&g2](const Disk<T> &d) {
                return g2 | match{
                        [&d](const Disk<T> &d2) {
                            return intersects(d, d2);
                        },
                        [&d](const Border<T> &b) {
                            return intersects(d, b);
                        }
                };
            },
            [&g2](const Border<T> &b) {
                return g2 | match{
                        [&b](const Disk<T> &d) {
                            return intersects(d, b);
                        },
                        [&b](const Border<T> &b2) {
                            return intersects(b, b2);
                        }
                };
            },
    };
}

template<class T>
TransformedVertex disk_to_transformed_vertex(const Disk<T> &disk, bool inbound) {
    return {disk.get_index(), inbound};
}


#endif //UTILS_GEOMETRY_OBJECTS_HPP