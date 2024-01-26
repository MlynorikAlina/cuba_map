//
// Created by alina-mlynorik on 7/27/23.
//
#include "MapFeatures.h"
using namespace std;

string map_features::to_string(map_features::MapFeature f) {

    switch (f) {
        case Aerialway:
            return "aerialway";
        case Aeroway:
            return "aeroway";
        case Amenity:
            return "amenity";
        case Barrier:
            return "barrier";
        case Boundary:
            return "boundary";
        case Building:
            return "building";
        case Craft:
            return "craft";
        case Ele:
            return "ele";
        case Emergency:
            return "emergency";
        case Geological:
            return "geological";
        case Healthcare:
            return "healthcare";
        case Highway:
            return "highway";
        case Historic:
            return "historic";
        case Landuse:
            return "landuse";
        case Leisure:
            return "leisure";
        case ManMade:
            return "man_made";
        case Military:
            return "military";
        case Natural:
            return "natural";
        case Office:
            return "office";
        case Place:
            return "place";
        case Power:
            return "power";
        case PublicTransport:
            return "public_transport";
        case Railway:
            return "railway";
        case Route:
            return "route";
        case Shop:
            return "shop";
        case Sport:
            return "sport";
        case Telecom:
            return "telecom";
        case Tourism:
            return "tourism";
        case Water:
            return "water";
        case Waterway:
            return "waterway";
        default:
            return "error";
    }
}

map_features::MapFeature map_features::from_string(string s) {
    if(s.compare("aerialway") == 0)
        return Aerialway;
    if(s.compare("aeroway") == 0)
        return Aeroway;
    if(s.compare("amenity") == 0)
        return Amenity;
    if(s.compare("barrier") == 0)
        return Barrier;
    if(s.compare("boundary") == 0)
        return Boundary;
    if(s.compare("building") == 0)
        return Building;
    if(s.compare("craft") == 0)
        return Craft;
    if(s.compare("ele") == 0)
        return Ele;
    if(s.compare("emergency") == 0)
        return Emergency;
    if(s.compare("geological") == 0)
        return Geological;
    if(s.compare("healthcare") == 0)
        return Healthcare;
    if(s.compare("highway") == 0)
        return Highway;
    if(s.compare("historic") == 0)
        return Historic;
    if(s.compare("landuse") == 0)
        return Landuse;
    if(s.compare("leisure") == 0)
        return Leisure;
    if(s.compare("man_made") == 0)
        return ManMade;
    if(s.compare("military") == 0)
        return Military;
    if(s.compare("natural") == 0)
        return Natural;
    if(s.compare("office") == 0)
        return Office;
    if(s.compare("place") == 0)
        return Place;
    if(s.compare("power") == 0)
        return Power;
    if(s.compare("public_transport") == 0)
        return PublicTransport;
    if(s.compare("railway") == 0)
        return Railway;
    if(s.compare("route") == 0)
        return Route;
    if(s.compare("shop") == 0)
        return Shop;
    if(s.compare("sport") == 0)
        return Sport;
    if(s.compare("telecom") == 0)
        return Telecom;
    if(s.compare("tourism") == 0)
        return Tourism;
    if(s.compare("water") == 0)
        return Water;
    if(s.compare("waterway") == 0)
        return Waterway;
    return static_cast<MapFeature>(-1);
}

set<string> map_features::getStringAll(set<MapFeature> features) {
    set<string> s;
    for (auto f:features)
        s.insert(to_string(f));
    return s;
}

set<string> map_features::getStringAllExcept(set<MapFeature> features) {
    set<string> s;
    for (int i = Aerialway; i <= Waterway; ++i)
        if(features.find(MapFeature(i)) == features.end())
            s.insert(to_string(MapFeature(i)));
    return s;
}

bool map_features::featureExists(string f) {
    return mapFeatureMapping.find(f) != mapFeatureMapping.end();
}

/*
vector<string> map_features::getOptions(map_features::MapFeature feature) {
    if (featureOptions.find(feature) != featureOptions.end()) {
        return featureOptions[feature].getOptions();
    }
    //TODO:: Optional, ptr instead of obj&
    return vector<string>();
}*/

set<map_features::MapFeature> map_features::getAllExcept(set<MapFeature> features) {
    set<MapFeature> s;
    for (int i = Aerialway; i <= Waterway; ++i)
        if(features.find(MapFeature(i)) == features.end())
            s.insert(MapFeature(i));
    return s;
}


set<map_features::MapFeature> map_features::getAll()
{
    set<MapFeature> s;
    for (int i = Aerialway; i <= Waterway; ++i)
        s.insert(MapFeature(i));
    return s;
}

set<string> map_features::getInfoTags()
{
    return infoTags;
}

bool map_features::isInfoTag(string tag)
{
    return infoTags.find(tag) != infoTags.end();
}
