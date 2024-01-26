//
// Created by alina-mlynorik on 7/28/23.
//

#ifndef MAPDOWNLOADER_SVGPAINTER_H
#define MAPDOWNLOADER_SVGPAINTER_H

#include <string>
#include <vector>
#include <fstream>
#include <ostream>
#include "../Geometry.h"


class SVGPainter {
private:
    std::string cssFileName;
    std:: ofstream svgFile;
    std::ofstream cssFile;
    std::string pathToCSSFile;
public:
    const int WIDTH;
    const int HEIGHT;
    class SVGPoint;


    SVGPainter(const std::string &SVGFileName, int width, int height);

    SVGPainter(const std::string &SVGFileName, const std::string &CSSFileName, int width, int height);

    void start();

    void polygon(const std::vector<SVGPoint> &points, const std::string &_class = "", const std::string &style = "");

    void circle(SVGPoint center, int radius, const std::string &_class = "", const std::string &style = "");

    void ellipse(SVGPoint center, int rx, int ry, const std::string &_class = "", const std::string &style = "");
    void ellipseRotated(SVGPoint f1, SVGPoint f2, const std::string &_class = "", const std::string &style = "");

    void rect(SVGPoint coordinates, int width, int height, const std::string &_class = "", int rx = 0, int ry = 0,
              const std::string &style = "");

    void line(SVGPoint pnt1, SVGPoint pnt2, const std::string &_class = "", const std::string &style = "");

    void polyline(const std::vector<SVGPoint> &points, const std::string &_class = "", const std::string &style = "");

    void text(SVGPoint coordinates, const std::string &text, const std::string &_class = "", const std::string &style = "");

    void image(SVGPoint coordinates, int width, int height, const std::string &href, const std::string &_class = "",
               const std::string &style = "");
    void addStyleToCSS(const std::string& styleCSS);
    void addPatternsFromFile(const std::string& paternsFile);
    static void changeStyleInSVG(const std::string &svgFileName, const std::string &styleCSS);
    void end();

    class SVGPoint : geom::Point{
    public:
        SVGPoint(double x, double y);
        SVGPoint(std::pair<double,double> p);
        SVGPoint(geom::Point p);

        friend std::ostream &operator<<(std::ostream &os, const SVGPoint &point);

        int getX() const;

        int getY() const;
    };

    void addToSVG(const std::string &data);
};


#endif //MAPDOWNLOADER_SVGPAINTER_H
