//
// Created by alina-mlynorik on 7/28/23.
//

#ifndef MAPDOWNLOADER_OSMPARSER_H
#define MAPDOWNLOADER_OSMPARSER_H

#include <fstream>
#include "pugixml.hpp"
#include "OSMData.h"
#include <list>

using namespace std;



class OSMParser {
private:
    ifstream osmFile;
    pugi::xml_document doc;
    pugi::xml_node osmNode;
    OverpassFilter filter;

    set<string> features;

    bool isIncluded(string k, string v);
public:
    explicit OSMParser(const string &osmFileName, const OverpassFilter& filter);
    explicit OSMParser(const OverpassFilter& filter);

    void setOsmFile(const string &osmFileName);

    void setFilterBbox(double minLon, double minLat, double maxLon, double maxLat);
    Bbox getFilterBbox();

    //TODO::descriptor
    map<string, NodeAttributes> getNodes();
    list<pair<string, WayData>>  getWays(const map<string, NodeAttributes>& nodes);
    void appendNodes(map<string, NodeAttributes>& nodes);
    void appendWays(list<pair<string, WayData>>& ways, const map<string, NodeAttributes>& nodes);
    void appendRel(map<string, RelData> &rel);
};

struct wc {
    bool operator()(const pair<string, WayData> &a, const pair<string, WayData> &b);
};
#endif //MAPDOWNLOADER_OSMPARSER_H
