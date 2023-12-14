//
// Created by alina-mlynorik on 8/17/23.
//

#include <cmath>
#include "Geometry.h"

geom::Point::Point(double x, double y) : x(x), y(y) {}

double geom::Point::getX() const {
    return x;
}

double geom::Point::getY() const {
    return y;
}

std::ostream &operator<<(std::ostream &os, const geom::Point &point) {
    os << point.getX() << "," << point.getY();
    return os;
}

void geom::Point::setX(double x) {
    Point::x = x;
}

void geom::Point::setY(double y) {
    Point::y = y;
}

geom::Point::Point() {}

int geom::Point::getXi() const {
    return std::round(x);
}

int geom::Point::getYi() const {
    return std::round(y);
}

geom::Box::Box(Point p1, Point p2): defined(true) {
    border[0] = p1;
    border[1] = p2;
}

double geom::Box::sizeX() {
    return border[1].getX() - border[0].getX();
}

double geom::Box::sizeY() {
    return border[0].getY() - border[1].getY();
}

void geom::Box::setBox(Point p1, Point p2) {
    defined = true;
    border[0] = p1;
    border[1] = p2;
}

const geom::Point *geom::Box::getBorder() const {
    return border;
}

geom::Box::Box(): defined(false) {}

std::string geom::Box::to_string() {
    return std::to_string(border[0].getX()) + "," + std::to_string(border[0].getY()) + "," + std::to_string(border[1].getX()) + "," + std::to_string(border[1].getY());
}
