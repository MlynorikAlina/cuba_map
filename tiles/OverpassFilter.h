//
// Created by alina-mlynorik on 7/26/23.
//
#include <set>
#include "MapFeatures.h"

#define mf map_features

#ifndef MAPDOWNLOADER_OVERPASSFILTER_H
#define MAPDOWNLOADER_OVERPASSFILTER_H

using namespace std;

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

    set<mf::MapFeature> includedFeatures;
    set<mf::MapFeature> excludedFeatures;
    map<mf::MapFeature, set<string> > includedFeatureOptions;
    map<mf::MapFeature, set<string> > excludedFeatureOptions;

    string to_string(string additional);

    static void printFeatureOptions(ostream &os, const set<string>& opt);
public:
    OverpassFilter();

    void includeFeatures(const set<mf::MapFeature>& fs);

    void excludeFeatures(const set<mf::MapFeature>& fs);

    void includeFeatureOptions(mf::MapFeature f, set<string> fOpt);

    void excludeFeatureOptions(mf::MapFeature  f, set<string> fOpt);

    void setBbox(double minLon, double minLat, double maxLon, double maxLat);

    void setArea( double centerLat, double centerLon, double radius);

    bool isDisplayNoTags() const;

    void setDisplayNoTags(bool noTags);

    bool isBox() const;

    bool isRound() const;

    const Bbox &getBbox() const;

    const RoundArea &getArea() const;

    const set<map_features::MapFeature> &getIncludedFeatures() const;

    const set<map_features::MapFeature> &getExcludedFeatures() const;

    const map<map_features::MapFeature, set<string>> &getIncludedFeatureOptions() const;

    const map<map_features::MapFeature, set<string>> &getExcludedFeatureOptions() const;

    string getQuery();
};


#endif //MAPDOWNLOADER_OVERPASSFILTER_H
