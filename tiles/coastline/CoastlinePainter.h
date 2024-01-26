//
// Created by alina-mlynorik on 8/30/23.
//

#ifndef MAPDOWNLOADER_COASTLINEPAINTER_H
#define MAPDOWNLOADER_COASTLINEPAINTER_H
#include <fstream>
#include <tiles/OverpassFilter.h>
#include "../svg_converter/SVGPainter.h"

class CoastlinePainter {
private:
    std::ifstream texture;
    Bbox box;
public:
    explicit CoastlinePainter(const std::string& textureFile);
    void draw(SVGPainter *painter, int width, int height);
    const Bbox &getBox() const;
};


#endif //MAPDOWNLOADER_COASTLINEPAINTER_H
