//
// Created by alina-mlynorik on 8/11/23.
//

#include <set>
#include <iostream>
#include <stack>
#include <list>
#include "GrahamMVO.h"

double GrahamMVO::rotate(const Point *p1, const Point *p2, const Point *p3)  {
    return (p2->getX()-p1->getX())*(p3->getY()-p2->getY())-(p2->getY()-p1->getY())*(p3->getX()-p2->getX());
}

GrahamMVO::GrahamMVO() {}


bool GrahamMVO::GrahamMVOCmp::operator()(const Point* lhs, const Point* rhs) const {
    if(rhs==startPoint)return true;
    return rotate(startPoint,lhs,rhs)>0;
}

GrahamMVO::GrahamMVOCmp::GrahamMVOCmp(const Point *startPoint) : startPoint(startPoint) {}
