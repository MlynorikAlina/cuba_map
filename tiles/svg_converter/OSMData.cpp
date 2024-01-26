//
// Created by alina-mlynorik on 7/31/23.
//

#include "OSMData.h"
using namespace std;

NodeAttributes::NodeAttributes(double lat, double lon) : lat(lat), lon(lon){}

double NodeAttributes::getLat() const {
    return lat;
}

double NodeAttributes::getLon() const {
    return lon;
}


WayAttributes::WayAttributes() {}

const map<string, string> &WayAttributes::getTags() const {
    return tags;
}

void WayAttributes::addTag(string key, string value) {
    tags.insert({{key, value}});
}

WayAttributes::WayAttributes(initializer_list<pair<const string, string>> list) : tags(list) {
}

bool WayAttributes::containsTag(const string &key) {
    return tags.find(key) != tags.end();
}

ostream &operator<<(ostream &os, const WayAttributes &attributes) {
    for(auto tag: attributes.tags)
        os<<tag.first<<"="<<tag.second<<" ";
    return os;
}

WayData::WayData() {_isArea = false;}

WayData::WayData(const WayAttributes &attributes) : attributes(attributes), _isArea(false) {}

WayData::WayData(const WayAttributes &attributes, const vector<const NodeAttributes *> &nodeRefs) : attributes(attributes),
                                                                                                    nodeRefs(nodeRefs), _isArea(false) {}

void WayData::addNodeRef(const NodeAttributes *ref) {
    if (ref != nullptr)
        nodeRefs.push_back(ref);
    //TODO::exception
}

void WayData::addTag(string key, string value) {
    attributes.addTag(key, value);
}

ostream &operator<<(ostream &os, const WayData &data) {
    os << "attributes: " << data.attributes << " nodeRefs: ";
    for(auto nd:data.nodeRefs)
        os<<"ref="<<nd<<" ";
    return os;
}

const WayAttributes &WayData::getAttributes() const {
    return attributes;
}

const vector<const NodeAttributes *> &WayData::getNodeRefs() const {
    return nodeRefs;
}

bool WayData::isArea() const {
    return _isArea;
}

void WayData::setIsArea() {
    WayData::_isArea = true;
}

const Bbox &WayData::getBorder() const {
    return border;
}

void WayData::setBorder(const Bbox &border) {
    WayData::border = border;
}

double WayData::getSquare() const {
    return (border.maxLon - border.minLon)*(border.maxLat - border.maxLat);
}

WayData::WayData(const WayData &wd) {
    attributes = wd.attributes;
    nodeRefs = wd.nodeRefs;
    _isArea = wd._isArea;
    border = wd.border;
}

WayData WayData::operator=(const WayData &wd){
    attributes = wd.attributes;
    nodeRefs = wd.nodeRefs;
    _isArea = wd._isArea;
    border = wd.border;
    return *this;
}

void RelData::addTag(string key, string value)
{
    attributes.addTag(key, value);
}

void RelData::addNode(string node)
{
    nodeRefs.push_back(node);
}

void RelData::addWay(string way)
{
    wayRefs.push_back(way);
}

void RelData::addRel(string rel)
{
    relRefs.push_back(rel);
}

WayAttributes RelData::getAttributes() const
{
    return attributes;
}

vector<string> RelData::getNodeRefs() const
{
    return nodeRefs;
}

vector<string> RelData::getWayRefs() const
{
    return wayRefs;
}

vector<string> RelData::getRelRefs() const
{
    return relRefs;
}
