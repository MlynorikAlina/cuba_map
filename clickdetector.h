#ifndef CLICKDETECTOR_H
#define CLICKDETECTOR_H


#include "qvector.h"

#include <QPoint>
class ClickDetector
{
public:
    ClickDetector();
    static bool onLine(QVector<double> &v, double x, double y);
    static double getCosA(double a_x, double a_y, double b_x, double b_y);
    static bool inPolygon(QVector<double> &v, double x, double y);
};

#endif // CLICKDETECTOR_H
