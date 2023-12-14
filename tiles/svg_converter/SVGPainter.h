//
// Created by alina-mlynorik on 7/28/23.
//

#ifndef MAPDOWNLOADER_SVGPAINTER_H
#define MAPDOWNLOADER_SVGPAINTER_H

#include <string>
#include <vector>
#include <fstream>
#include <ostream>
#include <filesystem>
#include "../Geometry.h"

using namespace std;

class SVGPainter {
private:
    string cssFileName;
    ofstream svgFile;
    ofstream cssFile;
    filesystem:: path pathToCSSFile;
public:
    const int WIDTH;
    const int HEIGHT;
    class SVGPoint;


    SVGPainter(const string &SVGFileName, int width, int height);

    SVGPainter(const string &SVGFileName, const string &CSSFileName, int width, int height);

    void start();

    void polygon(const vector<SVGPoint> &points, const string &_class = "", const string &style = "");

    void circle(SVGPoint center, int radius, const string &_class = "", const string &style = "");

    void ellipse(SVGPoint center, int rx, int ry, const string &_class = "", const string &style = "");
    void ellipseRotated(SVGPoint f1, SVGPoint f2, const string &_class = "", const string &style = "");

    void rect(SVGPoint coordinates, int width, int height, const string &_class = "", int rx = 0, int ry = 0,
              const string &style = "");

    void line(SVGPoint pnt1, SVGPoint pnt2, const string &_class = "", const string &style = "");

    void polyline(const vector<SVGPoint> &points, const string &_class = "", const string &style = "");

    void text(SVGPoint coordinates, const string &text, const string &_class = "", const string &style = "");

    void image(SVGPoint coordinates, int width, int height, const string &href, const string &_class = "",
               const string &style = "");
    void addStyleToCSS(const string& styleCSS);
    void addPatternsFromFile(const string& paternsFile);
    static void changeStyleInSVG(const string &svgFileName, const string &styleCSS);
    void end();

    class SVGPoint : geom::Point{
    public:
        SVGPoint(double x, double y);
        SVGPoint(pair<double,double> p);
        SVGPoint(geom::Point p);

        friend ostream &operator<<(ostream &os, const SVGPoint &point);

        int getX() const;

        int getY() const;
    };

    void addToSVG(const string &data);
};


#endif //MAPDOWNLOADER_SVGPAINTER_H
