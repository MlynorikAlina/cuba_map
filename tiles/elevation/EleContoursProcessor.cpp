//
// Created by alina-mlynorik on 8/15/23.
//

#include "EleContoursProcessor.h"
#include "../svg_converter/SVGPainter.h"
#include "GrahamMVO.h"
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <cmath>
#include <sstream>
#include <iomanip>

using namespace std;

EleContoursProcessor::EleContoursProcessor(const std::string &jsonFile) {
    if (QDir().exists(QString::fromStdString(jsonFile)))
        f.open(jsonFile);
}

geom::Point EleContoursProcessor::convert(geom::Point p, geom::Box &box,
                                          int width, int height) {
    return geom::Point(
        width * (p.getX() - box.getBorder()[0].getX()) / box.sizeX(),
        height * (1 - (box.getBorder()[0].getY() - p.getY()) / box.sizeY()));
}

void EleContoursProcessor::drawContoursOnload(const std::string &SVGFileName,
                                              int width, int height) {
    SVGPainter painter(SVGFileName, width, height);
    painter.start();
    painter.addToSVG("<style>\n"
                     "    path,circle,ellipse{\n"
                     "    stroke:red;\n"
                     "    stroke-width:0.5;\n"
                     "    fill:none;\n"
                     "    }\n"
                     "</style>");
    if (f.is_open()) {
        draw(painter, width, height);
    }
    painter.end();
}

EleContoursProcessor::~EleContoursProcessor() {
    if (f.is_open())
        f.close();
}

geom::Point EleContoursProcessor::countCenter(std::vector<geom::Point> points) {
    double sumx = 0, sumy = 0;
    for (auto p : points) {
        sumx += p.getX();
        sumy += p.getY();
    }

    return {sumx / points.size(), sumy / points.size()};
}

void EleContoursProcessor::drawInnerContour(SVGPainter &painter,
                                            std::vector<geom::Point> &MVO,
                                            int &width, int &height, double ele) {
    stringstream ss;
    stringstream t;
    t << fixed << setprecision(1) << ele;

    MVO.push_back(MVO[1]);
    bool q_ss = false;
    bool label_set = false;
    geom::Point p(round((MVO[0].getX() + MVO[1].getX()) / 2.),
                  round((MVO[0].getY() + MVO[1].getY()) /2.));
    ss << "<path d=\"M ";
    if (p.getYi() > 0 && p.getYi() < height - 2 && p.getXi() > 0 &&
        p.getXi() < width - 2) {
        ss << p.getXi() << "," << p.getYi() << " Q ";
        q_ss = true;
        painter.text({p.getX(), p.getY()}, t.str());
        label_set = true;
    }

    for (int i = 1; i < MVO.size() - 1; i++) {
        p = {round(MVO[i].getX()), round(MVO[i].getY())};
        ss << p.getXi() << "," << p.getYi() << " ";

        geom::Point q = geom::Point(
            round((MVO[i].getX() + MVO[(i + 1)].getX()) / 2),
            round((MVO[i].getY() + MVO[(i + 1)].getY()) / 2));
        if (q.getYi() > 0 && q.getYi() < height - 2 && q.getXi() > 0 &&
            q.getXi() < width - 2) {
            if (!q_ss) {
                ss << "Q ";
                ss << p.getXi() << "," << p.getYi() << " ";
                q_ss = true;
            }
            if (!label_set) {
                painter.text({q.getX(), q.getY()}, t.str());
                label_set = true;
            }
            ss << q.getXi() << "," << q.getYi() << " ";
        } else {
            ss << p.getXi() << "," << p.getYi() << " ";
            label_set = false;
            if (i < MVO.size() - 2) {
                ss << "M ";
                q_ss = false;
            }
        }
    }
    ss << "\" class=\"contours\"/>";
    painter.addToSVG(ss.str());
}

void EleContoursProcessor::draw(SVGPainter &painter, int &width, int &height) {
    while (!f.eof()) {
        string s;
        std::getline(f, s);
        if (s.size() > 0) {
            QJsonObject o = QJsonDocument::fromJson(QString::fromStdString(s).toUtf8()).object();
            double ele = o["ele"].toDouble();
            std::vector<Point> MVO;

            QJsonArray coordinates = o["coordinates"].toArray();
            for (int i = 0; i < coordinates.size(); ++i) {
                QJsonArray xy = coordinates[i].toArray();
                MVO.emplace_back(xy[0].toDouble(), xy[1].toDouble());
            }
            if (MVO.size() > 5)
                drawInnerContour(painter, MVO, width, height, ele);
        }
    }
}

void EleContoursProcessor::appContoursOnload(SVGPainter &painter, int width,
                                             int height) {
    if (f.is_open())
        draw(painter, width, height);
}
