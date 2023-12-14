#ifndef VECTOROSMLOADER_H
#define VECTOROSMLOADER_H

#include <QThread>

#include <tiles/OverpassFilter.h>

class VectorOsmLoader : public QThread
{
public:
    VectorOsmLoader(double clat, double clon, double max_dist);
    Bbox getBox() const;

public slots:
    void load();
    // QThread interface
protected:
    void run();

private:
    Bbox b;
    double dist;
};

#endif // VECTOROSMLOADER_H
