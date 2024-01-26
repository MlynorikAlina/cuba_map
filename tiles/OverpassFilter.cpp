//
// Created by alina-mlynorik on 7/26/23.
//

#include "OverpassFilter.h"
#include <sstream>
#include <iostream>
#include <utility>

using namespace std;

OverpassFilter::OverpassFilter() : displayNoTags(false), _isBox(false), _isRound(false) {}

string OverpassFilter::to_string(string additional) {
    stringstream ss;
    if (includedFeatures.empty() && includedFeatureOptions.empty())
        ss << "nwr" << additional << ";";
    else {
        for (auto el: includedFeatures) {
            ss << "nwr[" << mf::to_string(el) << "]" << additional << ";";
        }
        for (const auto &el: includedFeatureOptions) {
            ss << "nwr[" << mf::to_string(el.first) << "~\"";
            printFeatureOptions(ss, el.second);
            ss << "\"]" << additional << ";";
        }
    }
    ss<<");-(";
    if (!excludedFeatures.empty() || !excludedFeatureOptions.empty()) {
        for (auto el: excludedFeatures) {
            ss << "nwr[" << mf::to_string(el) << "]"<< additional << ";";
        }
        for (const auto &el: excludedFeatureOptions) {
            ss << "nwr[" << mf::to_string(el.first) << "~\"";
            printFeatureOptions(ss, el.second);
            ss << "\"]"<< additional << ";";
        }
    }
    return ss.str();
}

void OverpassFilter::printFeatureOptions(ostream &os, const set<string> &opt) {
    for (auto it = opt.begin(); it != opt.end();) {
        os << "(" << *it;
        if (++it != opt.end())
            os << ")|";
        else os << ")";
    }
}

void OverpassFilter::includeFeatures(const set<mf::MapFeature>& fs) {
    includedFeatures.insert(fs.begin(), fs.end());
}

void OverpassFilter::excludeFeatures(const set<mf::MapFeature>& fs) {
    excludedFeatures.insert(fs.begin(), fs.end());
}

//  https://maps.mail.ru/osm/tools/overpass/api/interpreter?data=[Bbox];node[amenity=post_box];out;&Bbox=7.0,50.6,7.3,50.8
string OverpassFilter::getQuery() {
    stringstream ss;
    if (_isBox) {
        ss << "data=[bbox:" << bbox.minLon << "," << bbox.minLat << "," << bbox.maxLon << "," << bbox.maxLat << "];(("
           << to_string("");
    } else {
        stringstream add;
        add << "(around:" << area.radius << "," << area.centerLat << "," << area.centerLon << ")";
        ss << "((" << to_string(add.str());
    }
    ss << ");)->.d;(.d;>;)->.dat;.dat;out;";

    return ss.str();
}

void OverpassFilter::includeFeatureOptions(map_features::MapFeature f, set<string> fOpt) {
    includedFeatureOptions[f] = std::move(fOpt);
}

void OverpassFilter::excludeFeatureOptions(map_features::MapFeature f, set<string> fOpt) {
    excludedFeatureOptions[f] = std::move(fOpt);
}

void OverpassFilter::setBbox(double minLon, double minLat, double maxLon, double maxLat) {
    _isBox = true;
    _isRound = false;
    bbox.minLon = minLon;
    bbox.minLat = minLat;
    bbox.maxLon = maxLon;
    bbox.maxLat = maxLat;
}

void OverpassFilter::setArea(double centerLat, double centerLon, double radius) {
    _isBox = false;
    _isRound = true;
    area.centerLat = centerLat;
    area.centerLon = centerLon;
    area.radius = radius;
}

const Bbox &OverpassFilter::getBbox() const {
    return bbox;
}

const RoundArea &OverpassFilter::getArea() const {
    return area;
}

bool OverpassFilter::isBox() const {
    return _isBox;
}

bool OverpassFilter::isRound() const {
    return _isRound;
}

const set<map_features::MapFeature> &OverpassFilter::getIncludedFeatures() const {
    return includedFeatures;
}

const set<map_features::MapFeature> &OverpassFilter::getExcludedFeatures() const {
    return excludedFeatures;
}

const map<map_features::MapFeature, set<string>> &OverpassFilter::getIncludedFeatureOptions() const {
    return includedFeatureOptions;
}

const map<map_features::MapFeature, set<string>> &OverpassFilter::getExcludedFeatureOptions() const {
    return excludedFeatureOptions;
}

void OverpassFilter::setDisplayNoTags(bool noTags) {
    OverpassFilter::displayNoTags = noTags;
}

bool OverpassFilter::isDisplayNoTags() const {
    return displayNoTags;
}


void Bbox::setBox(double latMin, double lonMin, double latMax, double lonMax) {
    maxLon = lonMax;
    maxLat = latMax;
    minLon = lonMin;
    minLat = latMin;
    defined = true;
}

bool Bbox::isIn(double x, double y) const
{
    return x>=minLon && x<=maxLon && y>=minLat &&y<=maxLat;
}
