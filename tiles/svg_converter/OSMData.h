//
// Created by alina-mlynorik on 7/31/23.
//

#ifndef MAPDOWNLOADER_OSMDATA_H
#define MAPDOWNLOADER_OSMDATA_H

#include <map>
#include <string>
#include <vector>
#include <ostream>
#include "../OverpassFilter.h"

using namespace std;

class NodeAttributes {
private:
    double lat;
    double lon;
public:
    NodeAttributes(double lat, double lon);


    double getLat() const;

    double getLon() const;
};

class WayAttributes {
private:
    map<string, string> tags;
public:
    WayAttributes();

    WayAttributes(initializer_list<pair<const string, string>> list);

    const map<string, string> &getTags() const;//TODO::change to multimap???
    bool containsTag(const string &key);

    void addTag(string key, string value);

    friend ostream &operator<<(ostream &os, const WayAttributes &attributes);
};

class WayData {
private:
    WayAttributes attributes;
    vector<const NodeAttributes *> nodeRefs;
    bool _isArea;
    Bbox border;
public:
    WayData();

    explicit WayData(const WayAttributes &attributes);

    WayData(const WayAttributes &attributes, const vector<const NodeAttributes *> &nodeRefs);
    WayData(const WayData& wd);

    void addNodeRef(const NodeAttributes *ref);

    void addTag(string key, string value);

    const WayAttributes &getAttributes() const;

    const vector<const NodeAttributes *> &getNodeRefs() const;

    friend ostream &operator<<(ostream &os, const WayData &data);

    WayData operator=(const WayData &wd);

    bool isArea() const;

    void setIsArea();

    const Bbox &getBorder() const;

    void setBorder(const Bbox &border);
    double getSquare() const;
};

class RelData{
private:
    WayAttributes attributes;
    vector<string> nodeRefs;
    vector<string> wayRefs;
    vector<string> relRefs;
public:
    RelData() {}
    void addTag(string key, string value);
    void addNode(string node);
    void addWay(string way);
    void addRel(string rel);

    WayAttributes getAttributes() const;
    vector<string> getNodeRefs() const;
    vector<string> getWayRefs() const;
    vector<string> getRelRefs() const;
};

#endif //MAPDOWNLOADER_OSMDATA_H
