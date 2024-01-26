//
// Created by alina-mlynorik on 8/7/23.
//

#include <GL/glew.h>
#include <algorithm>
#include "GraphicPrimitives.h"

namespace grp {
    Point::Point(double x, double y) : x(x), y(y) {}

    Point::Point() {}

    Box::Box(double x1, double y1, double x2, double y2) : p1(x1, y1), p2(x2, y2) {}

    Box::Box(const Point &p1, const Point &p2) : p1(p1), p2(p2) {}

    const Point &Box::getP1() const {
        return p1;
    }

    const Point &Box::getP2() const {
        return p2;
    }

    const double Box::getDiffY() const {
        return p1.y - p2.y;
    }

    const double Box::getDiffX() const {
        return p2.x - p1.x;
    }

    bool Box::isIn(Point p) const {
        return p.x > p1.x && p.x < p2.x && p.y > p2.y && p.y < p1.y;
    }

    void Box::initGLArrays(unsigned int &VAO, unsigned int &VBO) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(double) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(double), (void *) 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Box::drawBox(unsigned int &VAO, unsigned int &VBO) const {
        glBindVertexArray(VAO);
        GLdouble vertices[6][4] = {
                {p1.x, p1.y, 0.0, 0.0},
                {p1.x, p2.y, 0.0, 1.0},
                {p2.x, p2.y, 1.0, 1.0},

                {p1.x, p1.y, 0.0, 0.0},
                {p2.x, p1.y, 1.0, 0.0},
                {p2.x, p2.y, 1.0, 1.0}
        };
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices),
                        vertices); // Be sure to use glBufferSubData and not glBufferData
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    Box::Box() {}

    double &Box::getP2Y() {
        return p2.y;
    }

    void Box::setBox(double x1, double y1, double x2, double y2) {
        p1.x = x1;
        p1.y = y1;
        p2.x = x2;
        p2.y = y2;
    }

    const Point &Triangle::getP1() const {
        return p1;
    }

    const Point &Triangle::getP2() const {
        return p2;
    }

    const Point &Triangle::getP3() const {
        return p3;
    }

    bool Triangle::isIn(Point p) const {
        return border.isIn(p);
    }

    Triangle::Triangle(double x1, double y1, double x2, double y2, double x3, double y3) : p1(x1, y1), p2(x2, y2),
                                                                                     p3(x3, y3) {
        border = Box(std::min(std::min(x1, x2), x3), std::max(std::max(y1, y2), y3), std::max(std::max(x1, x2), x3),
                     std::min(std::min(y1, y2), y3));
    }

    Triangle::Triangle(const Point &p1, const Point &p2, const Point &p3) : p1(p1), p2(p2), p3(p3) {
        border = Box(std::min(std::min(p1.x, p2.x), p3.x), std::max(std::max(p1.y, p2.y), p3.y),
                     std::max(std::max(p1.x, p2.x), p3.x), std::min(std::min(p1.y, p2.y), p3.y));
    }

    Triangle::Triangle() {}

    void Triangle::drawTriangle(unsigned int &VAO, unsigned int &VBO) const {
        glBindVertexArray(VAO);
        GLdouble vertices[3][2] = {
                {p1.x, p1.y},
                {p2.x, p2.y},
                {p3.x, p3.y},
        };
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices),
                        vertices); // Be sure to use glBufferSubData and not glBufferData
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void Triangle::initGLArrays(unsigned int &VAO, unsigned int &VBO) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(double) * 3 * 2, NULL, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(double), (void *) 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    Line::Line(const Point &p1, const Point &p2) : p1(p1), p2(p2) {}

    Line::Line(double x1, double y1, double x2, double y2) : p1(x1, y1), p2(x2, y2) {}

    void Line::initGLArrays(unsigned int &VAO, unsigned int &VBO) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(double) * 2 * 2, NULL, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(double), (void *) 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Line::drawLine(unsigned int &VAO, unsigned int &VBO) const {
        glBindVertexArray(VAO);
        GLdouble vertices[2][2] = {
                {p1.x, p1.y},
                {p2.x, p2.y}
        };
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices),
                        vertices); // Be sure to use glBufferSubData and not glBufferData
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_LINES, 0, 2);
    }
    TileData::TileData(int layer, int contoursTick, int contourDist, int precision, double tileSizeDeg) : layer(layer),
                                                                                                          contoursTick(
                                                                                                                  contoursTick),
                                                                                                          contourDist(
                                                                                                                  contourDist),
                                                                                                          precision(
                                                                                                                  precision),
                                                                                                          tileSizeDeg(
                                                                                                                  tileSizeDeg) {}
} // grp
