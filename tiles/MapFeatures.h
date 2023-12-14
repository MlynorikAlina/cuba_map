//
// Created by alina-mlynorik on 7/26/23.
//

#ifndef MAPDOWNLOADER_MAPFEATURES_H
#define MAPDOWNLOADER_MAPFEATURES_H

#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;

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

    string to_string(MapFeature f);
    MapFeature from_string(string s);
    set<string> getStringAllExcept(set<MapFeature> features);
    set<string> getStringAll(set<MapFeature> features);
    set<string> getInfoTags();
    bool isInfoTag(string tag);
    bool featureExists(string f);
    set<MapFeature> getAll();
    set<MapFeature> getAllExcept(set<MapFeature> features);

    namespace {
        map<string, MapFeature> initializeToMapFeature(){
            map<string, MapFeature> m;
            for (int i = Aerialway; i <= Waterway; ++i)
                m[to_string(MapFeature(i))] = MapFeature(i);
            return m;
        }

        map<string, MapFeature> mapFeatureMapping = initializeToMapFeature();
        set<string> infoTags = {"changeset", "timestamp", "version"};
    }

    //TODO::FEATURES_AND_OPTIONS_VALIDATION
    /*namespace {
        class FeatureOptions {
        private:
            MapFeature feature;
            vector<string> options;
        public:
            explicit FeatureOptions(MapFeature feature) : feature(feature) {
                switch (feature) {
                    case Boundary:
                        options = {"administrative", "national_park", "protected_area"};
                        break;
                }
            };

            const vector<string> &getOptions() const {
                return options;
            }

            FeatureOptions() {
            }
        };

        map<MapFeature, FeatureOptions> initializeAllFeatures() {
            map<MapFeature, FeatureOptions> featureOptions;
            for (int i = Aerialway; i <= Waterway; ++i)
                featureOptions[MapFeature(i)] = FeatureOptions(MapFeature(i));
            return featureOptions;
        }


        map<MapFeature, FeatureOptions> featureOptions = initializeAllFeatures();
    }

    vector<string> getOptions(MapFeature feature);
    */

};
#endif //MAPDOWNLOADER_MAPFEATURES_H
