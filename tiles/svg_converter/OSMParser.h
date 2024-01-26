//
// Created by alina-mlynorik on 7/28/23.
//

#ifndef MAPDOWNLOADER_OSMPARSER_H
#define MAPDOWNLOADER_OSMPARSER_H

#include <fstream>
#include "pugixml.hpp"
#include "OSMData.h"
#include <list>



class OSMParse {
private:
    std::ifstream osmFile;
    pugi::xml_document doc;
    pugi::xml_node osmNode;
    OverpassFilter filter;

    std::set<std::string> features;

    bool isIncluded(std::string k, std::string v);
public:
    explicit OSMParse(const std::string &osmFileName, const OverpassFilter& filter);
    explicit OSMParse(const OverpassFilter& filter);

    void setOsmFile(const std::string &osmFileName);

    void setFilterBbox(double minLon, double minLat, double maxLon, double maxLat);
    Bbox getFilterBbox();

    //TODO::descriptor
    std::map<std::string, NodeAttributes> getNodes();
    std::list<std::pair<std::string, WayData>>  getWays(const std::map<std::string, NodeAttributes>& nodes);
    void appendNodes(std::map<std::string, NodeAttributes>& nodes);
    void appendWays(std::list<std::pair<std::string, WayData>>& ways, const std::map<std::string, NodeAttributes>& nodes);
    void appendRel(std::map<std::string, RelData> &rel);
};

struct wc {
    bool operator()(const std::pair<std::string, WayData> &a, const std::pair<std::string, WayData> &b);
};
#endif //MAPDOWNLOADER_OSMPARSER_H
