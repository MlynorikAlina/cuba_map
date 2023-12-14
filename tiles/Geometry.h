//
// Created by alina-mlynorik on 8/17/23.
//

#ifndef MAPDOWNLOADER_GEOMETRY_H
#define MAPDOWNLOADER_GEOMETRY_H

#include <ostream>
namespace geom {
    class Point {
    protected:
        double x;
        double y;
    public:
        Point();

        Point(double x, double y);

        void setX(double x);

        void setY(double y);

        friend std::ostream &operator<<(std::ostream &os, const Point &point);

        double getX() const;

        double getY() const;

        int getXi() const;

        int getYi() const;
    };

    class Box {
    private:
        Point border[2];
    public:
        bool defined;

        explicit Box(Point p1, Point p2);

        Box();

        const Point *getBorder() const;

        std::string to_string();

        void setBox(Point p1, Point p2);

        double sizeX();

        double sizeY();
    };
}
#endif //MAPDOWNLOADER_GEOMETRY_H
