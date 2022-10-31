#ifndef BARRIER_RESILIENCE_VISUALIZE_HPP
#define BARRIER_RESILIENCE_VISUALIZE_HPP

#include <string>
#include <sstream>
#include "geometry_objects.hpp"

const int borderWidthThreshold = 1;
const int svgSizeMultiplier = 100;

// Converts objects to svg format.
// Far from perfect, but very useful for debugging.
// Returns svg string.
template<class T>
std::string objects_to_svg(const std::vector<GeometryObject<T>>& objects, std::vector<int> to_remove = {}) {
    std::stringstream ss;

    // Find min and max x and y coordinates.
    T min_x = std::numeric_limits<T>::max();
    T max_x = std::numeric_limits<T>::min();
    T min_y = std::numeric_limits<T>::max();
    T max_y = std::numeric_limits<T>::min();

    for (const GeometryObject<T>& object : objects) {
        // - Show at least a bit to each side of the border
        // - We want to show all disks
        min_x = std::min(min_x, object | match{
                [](const Border<int> &border) { return border.x - borderWidthThreshold; },
                [](const Disk<int> &disk) { return disk.center.x - disk.radius; },
        });
        max_x = std::max(max_x, object | match {
            [](const Border<int>& border) { return border.x + borderWidthThreshold; },
            [](const Disk<int>& disk) { return disk.center.x + disk.radius; },
        });
        min_y = std::min(min_y, object | match {
            [min_y](const Border<int>& border) { return min_y; },
            [](const Disk<int>& disk) { return disk.center.y - disk.radius; },
        });
        max_y = std::max(max_y, object | match {
            [max_y](const Border<int>& border) { return max_y; },
            [](const Disk<int>& disk) { return disk.center.y + disk.radius; },
        });
    }

    min_y -= 1;
    max_y += 1;

    min_x *= svgSizeMultiplier;
    max_x *= svgSizeMultiplier;
    min_y *= svgSizeMultiplier;
    max_y *= svgSizeMultiplier;

    ss << "<svg width=\"" << max_x - min_x
        << "\" height=\"" << max_y - min_y
        << "\" viewBox=\"0 0 " << max_x - min_x << " " << max_y - min_y
        << "\" xmlns=\"http://www.w3.org/2000/svg"
        << "\">" << std::endl;

    // Show objects.
    for (int i = 0; i < objects.size(); ++i) {
        const auto& object = objects[i];
        // Should be object marked as removed.
        bool to_remove_object = std::find(to_remove.begin(), to_remove.end(), i) != to_remove.end();

        object | match {
            [&ss, min_x, min_y, max_y](const Border<int>& border) {
                ss << "<line x1=\"" << svgSizeMultiplier * border.x - min_x
                    << "\" y1=\"" << min_y
                    << "\" x2=\"" << svgSizeMultiplier * border.x - min_x
                    << "\" y2=\"" << max_y - min_y
                    << "\" style=\"stroke:rgb(0,0,0);stroke-width:2\" />" << std::endl;
            },
            [&ss, min_x, min_y, to_remove_object](const Disk<int>& disk) {
                ss << "<circle cx=\"" << svgSizeMultiplier * disk.center.x - min_x
                    << "\" cy=\"" << svgSizeMultiplier * disk.center.y - min_y
                    << "\" r=\"" << svgSizeMultiplier * disk.radius
                    << "\" stroke=\"" << (to_remove_object ? "red" : "black")
                    << "\" stroke-width=\"1\" fill=\"none\" />" << std::endl;
            },
        };
    }
    ss << "</svg>" << std::endl;

    return ss.str();
}

// to_remove - indices of disk which need to be marked as removed (indices will stay the same because border is pushed to the end).
template<class T>
std::string problem_to_svg(const std::vector<Disk<T>>& disks, const int left_border_x, const int right_border_x, std::vector<int> to_remove = {}) {
    std::vector<GeometryObject<int>> objects;
    for (const Disk<int>& disk : disks) {
        objects.push_back(disk);
    }
    objects.push_back(Border<int>{left_border_x, true});
    objects.push_back(Border<int>{right_border_x, false});
    return objects_to_svg(objects, to_remove);
}

#endif // BARRIER_RESILIENCE_VISUALIZE_HPP
