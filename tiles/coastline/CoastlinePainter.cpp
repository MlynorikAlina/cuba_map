//
// Created by alina-mlynorik on 8/30/23.
//

#include <QJsonObject>
#include <sstream>
#include "CoastlinePainter.h"
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>

using namespace std;

CoastlinePainter::CoastlinePainter(const string &textureFile) {
    if (QDir().exists(QString::fromStdString(textureFile))){
        texture.open(textureFile);
        string s;
        std::getline(texture, s);
        QJsonObject o = QJsonDocument::fromJson(QString::fromStdString(s).toUtf8()).object();
        QJsonArray v = o["box"].toArray();
        box.setBox(v[0].toDouble(), v[1].toDouble(), v[2].toDouble(),
                   v[3].toDouble());
    }
}

void CoastlinePainter::draw(SVGPainter *painter, int width, int height) {
    if (texture.is_open()) {
        while (!texture.eof()) {
            string s;
            std::getline(texture, s);
            if (s.size() > 0) {
                std::vector<SVGPainter::SVGPoint> MVO;

                QJsonObject o = QJsonDocument::fromJson(QString::fromStdString(s).toUtf8()).object();
                QJsonArray coordinates = o["coordinates"].toArray();
                for (int i = 0; i < coordinates.size(); ++i) {
                    QJsonArray xy = coordinates[i].toArray();
                    MVO.emplace_back(xy[0].toDouble(), xy[1].toDouble());
                }
                painter->polyline(MVO, "natural-coastline");
            }
        }
    }
}

const Bbox &CoastlinePainter::getBox() const {
    return box;
}
