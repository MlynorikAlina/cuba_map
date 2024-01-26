//
// Created by alina-mlynorik on 7/26/23.
//
#include <set>
#include "MapFeatures.h"

#define mf map_features

#ifndef MAPDOWNLOADER_OVERPASSFILTER_H
#define MAPDOWNLOADER_OVERPASSFILTER_H

struct Bbox {
    double minLon;
    double minLat;
    double maxLon;
    double maxLat;
    double defined = false;

    void setBox(double latMin, double lonMin, double latMax, double lonMax);
    bool isIn(double x, double y) const;
};

struct RoundArea{
    double centerLon;
    double centerLat;
    double radius;
};

class OverpassFilter {
private:
    Bbox bbox;
    RoundArea area;
    bool _isBox;
    bool _isRound;
    bool displayNoTags;

    std::set<mf::MapFeature> includedFeatures;
    std::set<mf::MapFeature> excludedFeatures;
    std::map<mf::MapFeature, std::set<std::string> > includedFeatureOptions;
    std::map<mf::MapFeature, std::set<std::string> > excludedFeatureOptions;

    std::string to_string(std::string additional);

    static void printFeatureOptions(std::ostream &os, const std::set<std::string>& opt);
public:
    OverpassFilter();

    void includeFeatures(const std::set<mf::MapFeature>& fs);

    void excludeFeatures(const std::set<mf::MapFeature>& fs);

    void includeFeatureOptions(mf::MapFeature f, std::set<std::string> fOpt);

    void excludeFeatureOptions(mf::MapFeature  f, std::set<std::string> fOpt);

    void setBbox(double minLon, double minLat, double maxLon, double maxLat);

    void setArea( double centerLat, double centerLon, double radius);

    bool isDisplayNoTags() const;

    void setDisplayNoTags(bool noTags);

    bool isBox() const;

    bool isRound() const;

    const Bbox &getBbox() const;

    const RoundArea &getArea() const;

    const std::set<map_features::MapFeature> &getIncludedFeatures() const;

    const std::set<map_features::MapFeature> &getExcludedFeatures() const;

    const std::map<map_features::MapFeature, std::set<std::string>> &getIncludedFeatureOptions() const;

    const std::map<map_features::MapFeature, std::set<std::string>> &getExcludedFeatureOptions() const;

    std::string getQuery();
};


#endif //MAPDOWNLOADER_OVERPASSFILTER_H
