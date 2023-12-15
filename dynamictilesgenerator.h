#ifndef DYNAMICTILESGENERATOR_H
#define DYNAMICTILESGENERATOR_H

#include <QCheckBox>
#include <QThread>
#include <tiles/OverpassFilter.h>
#include "params.h"
enum UpdateMode {
    NO_UPDATE, UPDATE_FILTER, UPDATE_ALL
};

class DynamicTilesGenerator : public QThread
{
    Q_OBJECT
public:
    //DynamicTilesGenerator(QString osmFile, QString textureFile, QString outPngDir, OverpassFilter *filter, int lat, int lon, int size, double frac, int prec);
    DynamicTilesGenerator(const QString &dynamicMapDir, int size, const QVector<QString> &checkedDist);
signals:
    void tileGenerated();
public slots:
    void startPainting();
private:
    UpdateMode updateMapParams(QString fileOut, OverpassFilter *filter, QString lat, QString lon);
    QString dynamicMapDir;

    QString textureFile;
    int size;
    double frac;
    int prec;
    double lat;
    double lon;

    QString svgFile = QString(STYLE_DIR) + "test.svg";
    QVector<QString> checkedDist;

    // QThread interface
protected:
    void run();
};

#endif // DYNAMICTILESGENERATOR_H
