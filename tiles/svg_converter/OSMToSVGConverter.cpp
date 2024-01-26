//
// Created by alina-mlynorik on 7/28/23.
//

#include <cmath>
#include <stack>
#include <iostream>
#include <params.h>
#include <QDir>
#include "OSMToSVGConverter.h"
#include "../GraphicPrimitives.h"
#include <sstream>
#include <iomanip>

using namespace std;

double geoInfo::km2lat(double dist) {
    return dist / KM_IN_1_DEG_LAT;
}

double geoInfo::km2lon(double dist, double point_lon) {
    return dist / _kmIn1degLon(point_lon);
}

double geoInfo::_kmIn1degLon(double lon) {
    double lonrad = lon * M_PI / 180.0;
    return cos(lonrad) * 40075.696 / 360;
}

void OSMToSVGConverter::draw(const string &patternFile, double layerDist) {
    painter.start();
    if(QDir().exists(QString::fromStdString(patternFile)))
        painter.addPatternsFromFile(patternFile);
    painter.rect({0, 0}, painter.WIDTH, painter.HEIGHT, "osm");

    texturePainter.draw(&painter, painter.WIDTH, painter.HEIGHT);
    //for highway
    for (auto w: ways) {
        vector<SVGPainter::SVGPoint> points;
        stringstream _class;
        for (auto tg: w.second.getAttributes().getTags()) {
            if(map_features::featureExists(tg.first)){
                string temp =  tg.first;
                if(tg.second.find(' ')==string::npos&&tg.second.find('&')==string::npos){
                    temp+="-" + tg.second;
                    if(temp == "amenity-grave_yard")
                        temp = "grave_yard";
                }
                _class <<temp<< " ";
            }
        }
        int xmin=painter.WIDTH+1,xmax=-1,ymin=painter.HEIGHT+1,ymax=-1;

        for (auto n: w.second.getNodeRefs()) {
            SVGPainter::SVGPoint t(convertLon(n->getLon()),convertLat(n->getLat()));
            if(points.size()==0 || t.getX()!=points.rend()->getX()||t.getY()!=points.rend()->getY()){
                points.push_back(t);
                xmin = min(xmin, t.getX());
                ymin = min(ymin, t.getY());
                xmax = max(xmax, t.getX());
                ymax = max(ymax, t.getY());
            }
        }
        if((xmax-xmin)*(ymax-ymin)>MIN_SVG_ITEM_SQUARE)
            painter.polyline(points, _class.str());

    }
    if(elePainter!=NULL)
        elePainter->appContoursOnload(painter, painter.WIDTH, painter.HEIGHT);
    painter.end();
//    cerr<<"tile draw osm: "<<time(NULL)-st<<endl;
}

Bbox OSMToSVGConverter::getBounds() const
{
    return bounds;
}

double OSMToSVGConverter::convertLat(double lat) {
    return painter.HEIGHT - (lat - bounds.minLat) * LAT_TRANSFORM_COEFFICIENT;
}

double OSMToSVGConverter::convertLon(double lon) {
    return (lon - bounds.minLon) * LON_TRANSFORM_COEFFICIENT;
}


void OSMToSVGConverter::setBox(double latMin, double lonMin, double latMax, double lonMax) {
    bounds.minLat = latMin;
    bounds.minLon = lonMin;
    bounds.maxLat = latMax;
    bounds.maxLon = lonMax;
    LAT_TRANSFORM_COEFFICIENT = double(painter.WIDTH) / (bounds.maxLat - bounds.minLat);
    LON_TRANSFORM_COEFFICIENT = double(painter.HEIGHT) / (bounds.maxLon - bounds.minLon);
}
OSMToSVGConverter::OSMToSVGConverter(const string &osmFileName, const string &textureFile, OverpassFilter filter, const string &SVGFileName, const string &CSSFileName, int width, int height):
    parser(filter), painter(SVGFileName, CSSFileName, width, height),  texturePainter(textureFile)
{
    elePainter = NULL;

    parser.setOsmFile(osmFileName);

    parser.appendNodes(nodes);
    parser.appendWays(ways, nodes);

    setBox(parser.getFilterBbox());
    ways.sort(wc());
}

OSMToSVGConverter::OSMToSVGConverter(const string &osmFileName, const string &textureFile, OverpassFilter filter, const string &SVGFileName, const string &CSSFileName, Bbox box, int width, int height):
parser(filter), painter(SVGFileName, CSSFileName, width, height),  texturePainter(textureFile)
{
    elePainter = NULL;

    parser.setOsmFile(osmFileName);

    parser.appendNodes(nodes);
    parser.appendWays(ways, nodes);

    setBox(box);
    ways.sort(wc());
}

OSMToSVGConverter::OSMToSVGConverter(const string &osmFileDir, const string &contoursFile,
                                     const string &textureFile, OverpassFilter filter, const string &SVGFileName,
                                     const string &CSSFileName, int width, int height) :
                                     parser(filter), painter(SVGFileName, CSSFileName, width, height), texturePainter(textureFile) {
    elePainter = new EleContoursProcessor(contoursFile);
    setBox(texturePainter.getBox());
    for(double i = floor(bounds.minLat/TILE_STEP)*TILE_STEP+90;i<=floor(bounds.maxLat/TILE_STEP)*TILE_STEP+90;i+=TILE_STEP)
        for(double j = floor(bounds.minLon/TILE_STEP)*TILE_STEP+180;j<=floor(bounds.maxLon/TILE_STEP)*TILE_STEP+180;j+=TILE_STEP){
            parser.setFilterBbox(j, i, j + TILE_STEP, i + TILE_STEP);
            stringstream ss;
            ss<<fixed<<setprecision(TILE_STEP_PREC)<<osmFileDir<<i<<"/"<<j<<".osm";
            parser.setOsmFile(ss.str());
            parser.appendNodes(nodes);
            parser.appendWays(ways, nodes);
        }
    ways.sort(wc());
}

void OSMToSVGConverter::setBox(const Bbox &b) {
    setBox(b.minLat,b.minLon,b.maxLat,b.maxLon);
}
