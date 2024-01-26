//
// Created by alina-mlynorik on 7/26/23.
//

#ifndef MAPDOWNLOADER_MAPFEATURES_H
#define MAPDOWNLOADER_MAPFEATURES_H

#include <vector>
#include <string>
#include <map>
#include <set>

namespace map_features {
    enum MapFeature {
        Aerialway,
        Aeroway,
        Amenity,
        Barrier,
        Boundary,
        Building,
        Craft,
        Ele,
        Emergency,
        Geological,
        Healthcare,
        Highway,
        Historic,
        Landuse,
        Leisure,
        ManMade,
        Military,
        Natural,
        Office,
        Place,
        Power,
        PublicTransport,
        Railway,
        Route,
        Shop,
        Sport,
        Telecom,
        Tourism,
        Water,
        Waterway
    };

    std::string to_string(MapFeature f);
    MapFeature from_string(std::string s);
    std::set<std::string> getStringAllExcept(std::set<MapFeature> features);
    std::set<std::string> getStringAll(std::set<MapFeature> features);
    std::set<std::string> getInfoTags();
    bool isInfoTag(std::string tag);
    bool featureExists(std::string f);
    std::set<MapFeature> getAll();
    std::set<MapFeature> getAllExcept(std::set<MapFeature> features);

    namespace {
        std::map<std::string, MapFeature> initializeToMapFeature(){
            std::map<std::string, MapFeature> m;
            for (int i = Aerialway; i <= Waterway; ++i)
                m[to_string(MapFeature(i))] = MapFeature(i);
            return m;
        }

        std::map<std::string, MapFeature> mapFeatureMapping = initializeToMapFeature();
        std::set<std::string> infoTags = {"changeset", "timestamp", "version"};
    }

};
#endif //MAPDOWNLOADER_MAPFEATURES_H
