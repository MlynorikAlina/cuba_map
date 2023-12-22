//
// Created by alina-mlynorik on 8/15/23.
//

#ifndef ELEVATIONMAP_ELECONTOURSPROCESSOR_H
#define ELEVATIONMAP_ELECONTOURSPROCESSOR_H


#include <fstream>
#include <vector>
#include <map>
#include "../Geometry.h"
#include "../svg_converter/SVGPainter.h"
#include "../OverpassFilter.h"

class EleContoursProcessor {
private:
    std::ifstream f;
    void drawInnerContour(SVGPainter &painter, std::vector<geom::Point> &MVO, int &width,
                          int &height, double ele);
    void draw(SVGPainter &painter, int &width, int &height);
public:
    explicit EleContoursProcessor(const std::string& jsonFile);
    void drawContoursOnload(const std::string &SVGFileName, int width, int height);
    void appContoursOnload(SVGPainter &painter, int width, int height);
    geom::Point convert(geom::Point, geom::Box& box,int width, int height);
    geom::Point countCenter(std::vector<geom::Point> points);

    virtual ~EleContoursProcessor();
};


#endif //ELEVATIONMAP_ELECONTOURSPROCESSOR_H
