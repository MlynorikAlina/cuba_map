#ifndef STATICOSMLOADER_H
#define STATICOSMLOADER_H

#include <QThread>

#include <tiles/OverpassFilter.h>

class StaticOsmLoader : public QThread
{
public:
    explicit StaticOsmLoader(double clat, double clon, double max_dist);
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

#endif // STATICOSMLOADER_H
