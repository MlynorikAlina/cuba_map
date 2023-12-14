//
// Created by alina-mlynorik on 8/11/23.
//

#ifndef MAPDOWNLOADER_GRAHAMMVO_H
#define MAPDOWNLOADER_GRAHAMMVO_H


#include "../svg_converter/SVGPainter.h"
using namespace geom;

class GrahamMVO {

public:
    struct GrahamMVOCmp {
        const Point *startPoint;

        explicit GrahamMVOCmp(const Point *startPoint);

        bool operator()(const Point *lhs, const Point *rhs) const;
    };


    GrahamMVO();

    static double rotate(const Point *p1, const Point *p2, const Point *p3);

};


#endif //MAPDOWNLOADER_GRAHAMMVO_H
