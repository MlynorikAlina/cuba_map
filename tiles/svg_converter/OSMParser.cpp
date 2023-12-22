//
// Created by alina-mlynorik on 7/28/23.
//

#include <cstring>
#include <algorithm>
#include <iostream>
#include <list>
#include "OSMParser.h"


OSMParse::OSMParse(const string &osmFileName, const OverpassFilter &filter) : filter(filter) {
    if (filter.getIncludedFeatures().empty()) {
        features = map_features::getStringAllExcept(filter.getExcludedFeatures());
    } else {
        features = map_features::getStringAll(filter.getIncludedFeatures());
    }

    setOsmFile(osmFileName);

}

map<string, NodeAttributes> OSMParse::getNodes() {
    map<string, NodeAttributes> nodes;
    appendNodes(nodes);
    return nodes;
}



bool OSMParse::isIncluded(string k, string v) {

    if (!filter.getExcludedFeatureOptions().empty()) {
        auto it = filter.getExcludedFeatureOptions().find(mf::mapFeatureMapping[k]);
        if (it != filter.getExcludedFeatureOptions().end() &&
            it->second.find(v) != it->second.end())
            return false;
    }
    return true;
}

list<pair<string, WayData>> OSMParse::getWays(const map<string, NodeAttributes> &nodes) {
    list<pair<string, WayData>> ways;
    appendWays(ways, nodes);
    ways.sort(wc());
    return ways;
}


void OSMParse::setOsmFile(const string &osmFileName) {
    if(osmFile.is_open()){
        osmFile.close();
        doc.reset();
    }
    osmFile.open(osmFileName);
    pugi::xml_parse_result result = doc.load(osmFile);
    if (!result)
        printf("error ! can't load xml doc");
    osmNode = doc.child("osm");
    auto bounds = osmNode.child("bounds");
    if(!bounds.empty()){
        filter.setBbox(bounds.attribute("minlon").as_double(), bounds.attribute("minlat").as_double(), bounds.attribute("maxlon").as_double(), bounds.attribute("maxlat").as_double());
    }
    //TODO::exception
}

void OSMParse::setFilterBbox(double minLon, double minLat, double maxLon, double maxLat) {
    filter.setBbox(minLon, minLat, maxLon, maxLat);
}

Bbox OSMParse::getFilterBbox()
{
    return filter.getBbox();
}

void OSMParse::appendNodes(map<string, NodeAttributes> &nodes) {
    for (auto nd: osmNode.children("node")) {
        string id;
        int count = 0;
        double lat, lon;
        for (auto attr: nd.attributes()) {
            const char *attrName = attr.name();
            if (strcmp(attrName, "id") == 0) {
                id = attr.as_string();
                count++;
            }else if (strcmp(attrName, "lat") == 0) {
                lat = attr.as_double();
                count++;
            }else if (strcmp(attrName, "lon") == 0){
                lon = attr.as_double();
                count++;
            }
            if(count==0)
                break;
        }
        nodes.insert({id, NodeAttributes(lat, lon)});
    }
}

void OSMParse::appendWays(list<pair<string, WayData>> &ways, const map<string, NodeAttributes> &nodes) {
    string id;
    bool isHighway;
    string firstRef, lastRef;
    bool exclude;
    for (auto w: osmNode.children("way")) {
        WayData wd;
        Bbox bounds = {200, 100, -200, -100};
        isHighway = false;
        exclude = false;
        for (auto attr: w.attributes()) {
            string attrName = attr.name();
            if (attrName.compare("id") == 0)
                id = attr.as_string();
            else {
                if (map_features::featureExists(attrName)) {
                    if (!isIncluded(attrName, attr.value())) {
                        exclude = true;
                        break;
                    }
                }
                if(!map_features::isInfoTag(attr.name()))
                    wd.addTag(attr.name(), attr.value());

            }
        }
        if (exclude)
            continue;
        for (auto tg: w.children("tag")) {
            string k = tg.attribute("k").as_string();
            string v = tg.attribute("v").as_string();
            if (map_features::featureExists(k)) {
                if (!isIncluded(k, v)) {
                    exclude = true;
                    break;
                }
                if (k.compare("highway") == 0)
                    isHighway = true;
            }
            if(!map_features::isInfoTag(k))
                wd.addTag(k, v);
        }
        if (exclude)
            continue;
        if (!filter.isDisplayNoTags() && wd.getAttributes().getTags().empty())
            continue;
        for (auto nd: w.children("nd")) {
            auto it = nodes.find(nd.attribute("ref").as_string());
            if (it != nodes.end()) {
                wd.addNodeRef(&it->second);
                double lat = it->second.getLat();
                double lon = it->second.getLon();
                bounds.minLat = min(bounds.minLat, lat);
                bounds.minLon = min(bounds.minLon, lon);
                bounds.maxLat = max(bounds.maxLat, lat);
                bounds.maxLon = max(bounds.maxLon, lon);
            }
        }
        auto wdNodes = wd.getNodeRefs();
        if (!isHighway && wdNodes.size() > 1 && (*wdNodes.begin() == *wdNodes.rbegin())) {
            wd.setIsArea();
        }
        wd.setBorder(bounds);
        ways.emplace_back(id, wd);
    }
}

void OSMParse::appendRel(map<string, RelData>  &rel)
{

    string id;
    string firstRef, lastRef;
    bool exclude;
    for (auto r: osmNode.children("relation")) {
        RelData rd;
        exclude = false;
        for (auto attr: r.attributes()) {
            string attrName = attr.name();
            if (attrName.compare("id") == 0)
                id = attr.as_string();
            else {
                if (map_features::featureExists(attrName)) {
                    if (!isIncluded(attrName, attr.value())) {
                        exclude = true;
                        break;
                    }
                }
                if(!map_features::isInfoTag(attr.name()))
                    rd.addTag(attr.name(), attr.value());

            }
        }
        if (exclude)
            continue;
        for (auto tg: r.children("tag")) {
            string k = tg.attribute("k").as_string();
            string v = tg.attribute("v").as_string();
            if (map_features::featureExists(k)) {
                if (!isIncluded(k, v)) {
                    exclude = true;
                    break;
                }
            }
            if(!map_features::isInfoTag(k))
                rd.addTag(k, v);
        }
        if (exclude)
            continue;
        if (!filter.isDisplayNoTags() && rd.getAttributes().getTags().empty())
            continue;
        for (auto nd: r.children("member")) {
            string id = nd.attribute("ref").as_string();
            string type = nd.attribute("type").as_string();
            if(type == "way"){
                rd.addWay(id);
            }
            if(type == "relation"){
                rd.addRel(id);
            }
            if(type == "node"){
                rd.addNode(id);
            }
        }
        rel.insert({id, rd});
    }
}

OSMParse::OSMParse(const OverpassFilter &filter) {
    if (filter.getIncludedFeatures().empty()) {
        features = map_features::getStringAllExcept(filter.getExcludedFeatures());
    } else {
        features = map_features::getStringAll(filter.getIncludedFeatures());
    }
}


bool wc::operator()(const pair<string, WayData> &a, const pair<string, WayData> &b) {
    if (b.second.isArea()) {
        if (!a.second.isArea())
            return false;
        if (a.second.getSquare() <= b.second.getSquare()) {
            Bbox aBox = a.second.getBorder();
            Bbox bBox = b.second.getBorder();
            if (aBox.minLon >= bBox.minLon && aBox.maxLon <= bBox.maxLon &&
                aBox.minLat >= bBox.minLat && aBox.maxLat <= bBox.maxLat) {
                return false;
            } else {
                if ((aBox.minLon >= bBox.minLon && aBox.minLon <= bBox.maxLon) ||
                    (aBox.maxLon >= bBox.minLon && aBox.maxLon <= bBox.maxLon) ||
                    (aBox.minLat >= bBox.minLat && aBox.maxLat <= bBox.maxLat) ||
                    (aBox.maxLat >= bBox.minLat && aBox.maxLat <= bBox.maxLat))
                    return false;
            }
        }
    }
    return true;
}
