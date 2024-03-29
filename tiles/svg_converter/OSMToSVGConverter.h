//
// Created by alina-mlynorik on 7/28/23.
//

#ifndef MAPDOWNLOADER_OSMTOSVGCONVERTER_H
#define MAPDOWNLOADER_OSMTOSVGCONVERTER_H

#include "SVGPainter.h"
#include "OSMParser.h"
#include "../OverpassFilter.h"
#include "../elevation/EleContoursProcessor.h"
#include "../coastline/CoastlinePainter.h"

namespace geoInfo{
    const double KM_IN_1_DEG_LAT = 40008.55 / 360;

    double km2lat(double dist);

    double km2lon(double dist, double point_lon);

    double _kmIn1degLon(double lon);
}

class OSMToSVGConverter {
private:
    OSMParse parser;
    SVGPainter painter;
    EleContoursProcessor* elePainter;
    CoastlinePainter texturePainter;
    std::map<std::string,NodeAttributes> nodes;
    std::list<std::pair<std::string, WayData>>  ways;
    Bbox bounds;

    double LAT_TRANSFORM_COEFFICIENT;
    double LON_TRANSFORM_COEFFICIENT;
    double TRANSFORM_COEFFICIENT;
    //TODO::rename!
    double convertLat(double lat);
    double convertLon(double lon);
public:
    OSMToSVGConverter(const std::string &osmFileName, const std::string &textureFile, OverpassFilter filter,
                      const std::string &SVGFileName, const std::string &CSSFileName, int width, int height);
    OSMToSVGConverter(const std::string &osmFileName, const std::string &textureFile, OverpassFilter filter,
                      const std::string &SVGFileName, const std::string &CSSFileName, Bbox box, int width, int height);

    OSMToSVGConverter(const std::string &osmFileDir, const std::string &contoursFile,
                      const std::string &textureFile, OverpassFilter filter, const std::string &SVGFileName,
                      const std::string &CSSFileName, int width, int height);
    void draw(const std::string &patternFile, double layerDist);
    void setBox(double latMin, double lonMin, double latMax, double lonMax);
    void setBox(const Bbox &b);

    Bbox getBounds() const;
};


#endif //MAPDOWNLOADER_OSMTOSVGCONVERTER_H
