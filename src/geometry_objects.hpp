#ifndef BARRIER_RESILLIENCE_GEOMETRY_OBJECTS_HPP
#define BARRIER_RESILLIENCE_GEOMETRY_OBJECTS_HPP

template<class T>
struct Point{
    T x;
    T y;
};

template<class T>
struct Circle {
public:
    Point<T> center;
    T radius;
};

// Intersects returns true if the two circles have a non-empty intersection.
// (if one circle is contained in the other, they are considered to intersect)
template<class T>
bool intersects(const Circle<T>& c1, const Circle<T>& c2) {
    T dx = c1.center.x - c2.center.x;
    T dy = c1.center.y - c2.center.y;
    // Keep distance squared to avoid sqrt for better numerical stability.
    T d_squared = dx * dx + dy * dy;
    // Sum of radii squared.
    T r_squared = (c1.radius + c2.radius) * (c1.radius + c2.radius);
    return d_squared <= r_squared;
}

// Returns index of leftmost circle.
// If there are multiple leftmost circles, returns any of them.
// If there are no circles, returns -1.
template<class T>
int leftmost_circle(std::vector<Circle<T>>& circles) {
    int leftmost = -1;
    for (int i = 0; i < circles.size(); ++i) {
        if (leftmost == -1 || circles[i].center.x < circles[leftmost].center.x) {
            leftmost = i;
        }
    }
    return leftmost;
}

// Returns index of rightmost circle.
// Similar to leftmost_circle.
template<class T>
int rightmost_circle(std::vector<Circle<T>>& circles) {
    int rightmost = -1;
    for (int i = 0; i < circles.size(); ++i) {
        if (rightmost == -1 || circles[i].center.x > circles[rightmost].center.x) {
            rightmost = i;
        }
    }
    return rightmost;
}

#endif // BARRIER_RESILLIENCE_GEOMETRY_OBJECTS_HPP