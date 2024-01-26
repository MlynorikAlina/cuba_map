//
// Created by alina-mlynorik on 7/28/23.
//

#include <vector>
#include "SVGPainter.h"
#include <QDir>
#include <QFile>
#include <iostream>
#include <qdebug.h>
#include <qstring.h>
#include <valarray>

#define PI 3.14159265

using namespace std;


SVGPainter::SVGPainter(const string &SVGFileName, const int width, const int height) : WIDTH(width), HEIGHT(height) {
    svgFile.open(SVGFileName);
}

SVGPainter::SVGPainter(const string &SVGFileName, const string &CSSFileName, int width, int height) : WIDTH(width),
                                                                                                      HEIGHT(height),
                                                                                                      cssFileName(
                                                                                                      CSSFileName) {
    QDir dir(QString::fromStdString(SVGFileName));
    dir.setPath(dir.filePath(".."));
    dir.makeAbsolute();
    pathToCSSFile = dir.relativeFilePath(QDir(QString::fromStdString(CSSFileName)).absolutePath()).toStdString();
    svgFile.open(SVGFileName);
    cssFile.open(cssFileName, ios_base::app);
}

void SVGPainter::start() {
    if (cssFile.is_open()) {
        svgFile << R"(<?xml-stylesheet type="text/css" href=)" << pathToCSSFile << "?>" << endl;
    }
    svgFile << R"(<svg xmlns="http://www.w3.org/2000/svg" width=")" << WIDTH << "\" height=\"" << HEIGHT << "\" >"
            << endl;
}

void SVGPainter::end() {
    svgFile << "</svg>";
    svgFile.close();
    if (cssFile.is_open())
        cssFile.close();
}

SVGPainter::SVGPoint::SVGPoint(double x, double y) : Point(x,y) {}

ostream &operator<<(ostream &os, const SVGPainter::SVGPoint &point) {
    os << point.getX() << "," << point.getY();
    return os;
}

int SVGPainter::SVGPoint::getX() const {
    return round(x);
}

int SVGPainter::SVGPoint::getY() const {
    return round(y);
}

SVGPainter::SVGPoint::SVGPoint(pair<double, double> p) : Point(p.first,p.second){
}

SVGPainter::SVGPoint::SVGPoint(geom::Point p): Point(p) {
}

void SVGPainter::polygon(const vector<SVGPoint> &points, const string &_class, const string &style) {
    svgFile << "<polygon points=\"";
    for (auto point: points)
        svgFile << point << " ";
    svgFile << "\" class=\"" << _class << "\" style=\"" << style << "\" />" << endl;
}

void SVGPainter::circle(SVGPoint center, int radius, const string &_class, const string &style) {
    svgFile << "<circle cx=\"" << center.getX() << "\" cy=\"" << center.getY()
            << "\" r=\"" << radius << "\" class=\"" << _class << "\" style=\"" << style << "\" />" << endl;
}

void
SVGPainter::rect(SVGPoint coordinates, int width, int height, const string &_class, int rx, int ry, const string &style) {
    svgFile << "<rect x=\"" << coordinates.getX() << "\" y=\"" << coordinates.getY()
            << "\" width=\"" << width << "\" height=\"" << height << "\" rx=\"" << rx << "\" ry=\"" << ry
            << "\" class=\"" << _class << "\" style=\"" << style << "\" />" << endl;
}

void SVGPainter::polyline(const vector<SVGPoint> &points, const string &_class, const string &style) {
    svgFile << "<polyline points=\"";
    for (auto point: points)
        svgFile << point << " ";
    svgFile << "\" class=\"" << _class << "\" style=\"" << style << "\" />" << endl;
}

void SVGPainter::line(SVGPainter::SVGPoint pnt1, SVGPainter::SVGPoint pnt2, const string &_class, const string &style) {
    svgFile << "<line x1=\"" << pnt1.getX() << "\" y1=\"" << pnt1.getY() << "\" x2=\"" << pnt2.getX() << "\" y2=\""
            << pnt2.getY()
            << "\" class=\"" << _class << "\" style=\"" << style << "\" />" << endl;
}

void SVGPainter::ellipse(SVGPainter::SVGPoint center, int rx, int ry, const string &_class, const string &style) {
    svgFile << "<ellipse cx=\"" << center.getX() << "\" cy=\"" << center.getY() << "\" rx=\"" << rx << "\" ry=\"" << ry
            << "\" class=\"" << _class << "\" style=\"" << style << "\" />" << endl;
}

void SVGPainter::text(SVGPainter::SVGPoint coordinates, const string &text, const string &_class, const string &style) {
    svgFile << "<text x=\"" << coordinates.getX() << "\" y=\"" << coordinates.getY()
            << "\" class=\"" << _class << "\" style=\"" << style << "\">" << text << "</text>" << endl;
}

void SVGPainter::image(SVGPainter::SVGPoint coordinates, int width, int height, const string &href, const string &_class,
                       const string &style) {
    svgFile << "<image x=\"" << coordinates.getX() << "\" y=\"" << coordinates.getY()
            << "\" width=\"" << width << "\" height=\"" << height << "\" href=\"" << href
            << "\" class=\"" << _class << "\" style=\"" << style << "\" />" << endl;
}

void SVGPainter::addStyleToCSS(const string &styleCSS) {
    cssFile << styleCSS << endl;
}

void SVGPainter::addPatternsFromFile(const string &patternsFile) {
    ifstream is(patternsFile);
    string data;
    if (is.is_open()) {
        while (!is.eof()) {
            getline(is, data, '\n');
            addToSVG(data);
        }
    } else throw exception();
    //TODO::exception
}

void SVGPainter::addToSVG(const string &data) {
    svgFile << data << endl;
}

void SVGPainter::ellipseRotated(SVGPainter::SVGPoint f1, SVGPainter::SVGPoint f2, const string &_class, const string &style) {
    svgFile << "<ellipse cx=\"" << (f1.getX() + f2.getX()) / 2. << "\" cy=\"" << (f1.getY() + f2.getY()) / 2.
            << "\" rx=\"" << max((f2.getX() - f1.getX())/4.,(f2.getY() - f1.getY())/8.)
            << "\" ry=\"" << max((f2.getY() - f1.getY())/4., (f2.getX() - f1.getX())/8.)
            << "\" transform=\"rotate(" << 360. - atan2(f2.getY() - f1.getY(), f2.getX() - f1.getX()) * 180 / PI
            << ", "<< (f1.getX() + f2.getX()) / 2.<< ", "<< (f1.getY() + f2.getY()) / 2.<<")"
            << "\" class=\"" << _class << "\" style=\"" << style << "\" />" << endl;
}
void SVGPainter::changeStyleInSVG(const string &svgFileName, const string &styleCSS) {
    ifstream f(svgFileName);
    QString fn = QDir(QString::fromStdString(svgFileName)).dirName();
    string tfn = QString::fromStdString(svgFileName).replace(fn, "t"+fn).toStdString();
    ofstream t(tfn);
    string s;
    getline(f,s);
    t<<R"(<?xml-stylesheet type="text/css" href=)" <<styleCSS<< "?>" << endl;
    while (getline(f,s)){
        t<<s<<endl;
    }
    QDir(QString::fromStdString(svgFileName)).removeRecursively();
    QFile::rename(QString::fromStdString(tfn),QString::fromStdString(svgFileName));
}
