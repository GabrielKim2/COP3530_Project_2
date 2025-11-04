#ifndef POINT_H
#define POINT_H

#include <string>

struct Point {
    double x;
    double y;

    std::string label;

    Point(double x_val = 0.0, double y_val = 0.0, std::string _label = "") : x(x_val), y(y_val), label(_label) {}

    std::string toString() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
};

#endif