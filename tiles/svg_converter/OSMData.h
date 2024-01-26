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
    std::map<std::string, std::string> tags;
public:
    WayAttributes();

    WayAttributes(std::initializer_list<std::pair<const std::string, std::string>> list);

    const std::map<std::string, std::string> &getTags() const;//TODO::change to multimap???
    bool containsTag(const std::string &key);

    void addTag(std::string key, std::string value);

    friend std::ostream &operator<<(std::ostream &os, const WayAttributes &attributes);
};

class WayData {
private:
    WayAttributes attributes;
    std::vector<const NodeAttributes *> nodeRefs;
    bool _isArea;
    Bbox border;
public:
    WayData();

    explicit WayData(const WayAttributes &attributes);

    WayData(const WayAttributes &attributes, const std::vector<const NodeAttributes *> &nodeRefs);
    WayData(const WayData& wd);

    void addNodeRef(const NodeAttributes *ref);

    void addTag(std::string key, std::string value);

    const WayAttributes &getAttributes() const;

    const std::vector<const NodeAttributes *> &getNodeRefs() const;

    friend std::ostream &operator<<(std::ostream &os, const WayData &data);

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
    std::vector<std::string> nodeRefs;
    std::vector<std::string> wayRefs;
    std::vector<std::string> relRefs;
public:
    RelData() {}
    void addTag(std::string key, std::string value);
    void addNode(std::string node);
    void addWay(std::string way);
    void addRel(std::string rel);

    WayAttributes getAttributes() const;
    std::vector<std::string> getNodeRefs() const;
    std::vector<std::string> getWayRefs() const;
    std::vector<std::string> getRelRefs() const;
};

#endif //MAPDOWNLOADER_OSMDATA_H
