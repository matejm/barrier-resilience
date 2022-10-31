#ifndef BARRIER_RESILIENCE_GEOMETRY_OBJECTS_HPP
#define BARRIER_RESILIENCE_GEOMETRY_OBJECTS_HPP

#include <variant>
#include "utils.hpp"

template<class T>
struct Point{
    T x;
    T y;
};

// Disk with center and radius
template<class T>
struct Disk {
public:
    Point<T> center;
    T radius;
};

// Left or right border - vertical segment with given x coordinate
template<class T>
struct Border {
    T x;
    // Is this left or right border?
    bool left;
};

template<class T>
using GeometryObject = std::variant<Disk<T>, Border<T>>;

// Intersects returns true if the two disks have a non-empty intersection.
// (if one disk is contained in the other, they are considered to intersect)
template<class T>
bool intersects(const Disk<T>& d1, const Disk<T>& d2) {
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
bool intersects(const Disk<T>& d, const Border<T>& b) {
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
bool intersects(const Border<T>& b, const Disk<T>& d) {
    return intersects(d, b);
}

template<class T>
bool intersects(const Border<T>& b1, const Border<T>& b2) {
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
bool intersects(const GeometryObject<T>& g1, const GeometryObject<T>& g2) {
    return g1 | match {
        [&g2](const Disk<T>& d) {
            return g2 | match {
                [&d](const Disk<T>& d2) {
                    return intersects(d, d2);
                },
                [&d](const Border<T>& b) {
                    return intersects(d, b);
                }
            };
        },
        [&g2](const Border<T>& b) { 
            return g2 | match {
                [&b](const Disk<T>& d) {
                    return intersects(d, b);
                },
                [&b](const Border<T>& b2) {
                    return intersects(b, b2);
                }
            };
         },
    };
}

#endif // BARRIER_RESILIENCE_GEOMETRY_OBJECTS_HPP