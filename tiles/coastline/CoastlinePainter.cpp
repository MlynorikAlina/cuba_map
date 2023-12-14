//
// Created by alina-mlynorik on 8/30/23.
//

#include <sstream>
#include "CoastlinePainter.h"
#include <jsoncpp/json/json.h>

using namespace std;

CoastlinePainter::CoastlinePainter(const string &textureFile) {
    if (filesystem::exists(textureFile)){
        texture.open(textureFile);
        Json::Value v;
        string s;
        std::getline(texture, s);
        stringstream ssv(s);
        ssv >> v;
        v = v["box"];
        box.setBox(v[0].asDouble(), v[1].asDouble(), v[2].asDouble(),
                   v[3].asDouble());
    }
}

void CoastlinePainter::draw(SVGPainter *painter, int width, int height) {
    if (texture.is_open()) {
        while (!texture.eof()) {
            Json::Value v;
            string s;
            std::getline(texture, s);
            if (s.size() > 0) {
                stringstream ss(s);
                ss >> v;
                std::vector<SVGPainter::SVGPoint> MVO;

                Json::Value coordinates = v["coordinates"];
                for (int i = 0; i < coordinates.size(); ++i) {
                    Json::Value xy = coordinates[i];
                    MVO.emplace_back(xy[0].asDouble(), xy[1].asDouble());
                }
                painter->polyline(MVO, "natural-coastline");
            }
        }
    }
}

const Bbox &CoastlinePainter::getBox() const {
    return box;
}
