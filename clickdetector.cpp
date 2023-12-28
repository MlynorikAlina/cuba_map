#include "clickdetector.h"
#include "qdebug.h"
#include <math.h>

ClickDetector::ClickDetector()
{

}

bool ClickDetector::onLine(QVector<double> &v, double x, double y)
{
    double r = 5;
    for(int i=2; i<v.size(); i+=2){
        double k, p;
        if(v[i-2] == v[i])
            p = 0;
        else {
            k = (v[i-1] - v[i+1])/(v[i] - v[i-2]);
            p = sqrt(r*r/(1+k*k));
        }
        QVector<double> points;
        points.push_back((v[i-2] + p));points.push_back(((v[i-1] + p)));
        points.push_back((v[i] + p));points.push_back(((v[i+1] + p)));
        points.push_back((v[i] - p));points.push_back(((v[i+1] - p)));
        points.push_back((v[i-2] - p));points.push_back(((v[i-1] - p)));
        points.push_back(points[0]);points.push_back(points[1]);
        bool on = inPolygon(points, x, y);
        if(on)
            return true;
    }
    return false;
}

double ClickDetector::getCosA(double a_x, double a_y, double b_x, double b_y)
{
    double ta = sqrt(a_x*a_x+a_y*a_y);
    double tb = sqrt(b_x*b_x+b_y*b_y);
    double ab = a_x*b_x+a_y*b_y;
    return (ab)/(ta*tb);
}

bool ClickDetector::inPolygon(QVector<double> &v, double x, double y)
{
    double eps = 1e-5;
    double alph = 0;
    for(int i=2; i<v.size(); i+=2){
        double a_x = v[i-2]-x;
        double a_y = v[i-1]-y;
        double b_x = v[i]-x;
        double b_y = v[i+1]-y;
        double r = (getCosA(a_x, a_y, b_x, b_y));
        if(a_x*b_y-a_y*b_x >= 0)
            alph+=acos(r);
        else alph-=acos(r);
    }
    if(fabs(fabs(alph)-2*M_PI)<eps)
        return true;
    return false;
}
