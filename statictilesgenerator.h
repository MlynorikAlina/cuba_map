#ifndef STATICTILESGENERATOR_H
#define STATICTILESGENERATOR_H

#include <QThread>

#include <tiles/OverpassFilter.h>

class StaticTilesGenerator : public QThread
{

public:
    explicit StaticTilesGenerator(double lat, double lon, int size, const QVector<QString> &checkedDist, bool same, QString style);
    void load();
private:
    int size;
    double lat;
    double lon;
    QVector<QString> checkedDist;
    QString style;
    Bbox b;
    bool same;

    // QThread interface
protected:
    void run();
};

#endif // STATICTILESGENERATOR_H
