//
// Created by alina-mlynorik on 8/7/23.
//

#ifndef MAPGUI_GRAPHICPRIMITIVES_H
#define MAPGUI_GRAPHICPRIMITIVES_H

namespace grp {


    struct Point {
        double x;
        double y;

        Point(double x, double y);

        Point();
    };

    struct Box {
        Point p1;
        Point p2;

        Box();

        Box(double x1, double y1, double x2, double y2);

        Box(const Point &p1, const Point &p2);

        const Point &getP1() const;

        const Point &getP2() const;

        double &getP2Y();

        const double getDiffX() const;

        const double getDiffY() const;

        bool isIn(Point p) const;

        void drawBox(unsigned int &VAO, unsigned int &VBO) const;

        static void initGLArrays(unsigned int &VAO, unsigned int &VBO);

        void setBox(double x1, double y1, double x2, double y2);
    };

    struct Triangle {
        Point p1;
        Point p2;
        Point p3;
        Box border;

        Triangle(double x1, double y1, double x2, double y2, double x3, double y3);

        Triangle(const Point &p1, const Point &p2, const Point &p3);

        Triangle();

        const Point &getP1() const;

        const Point &getP2() const;

        const Point &getP3() const;

        bool isIn(Point p) const;

        void drawTriangle(unsigned int &VAO, unsigned int &VBO) const;

        static void initGLArrays(unsigned int &VAO, unsigned int &VBO);

    };

    struct Line {
        Point p1;
        Point p2;

        Line(const Point &p1, const Point &p2);

        Line(double x1, double y1, double x2, double y2);

        void drawLine(unsigned int &VAO, unsigned int &VBO) const;

        static void initGLArrays(unsigned int &VAO, unsigned int &VBO);
    };

    struct TileData{
        // tileSize in degrees, layer directory name, contours tick for layer
        int layer;
        int contoursTick;
        int contourDist;
        int precision;
        double tileSizeDeg;

        TileData(int layer, int contoursTick, int contourDist, int precision, double tileSizeDeg);

    };
} // grp

#endif //MAPGUI_GRAPHICPRIMITIVES_H
