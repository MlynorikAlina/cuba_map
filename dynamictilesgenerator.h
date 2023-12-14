#ifndef DYNAMICTILESGENERATOR_H
#define DYNAMICTILESGENERATOR_H

#include <QCheckBox>
#include <QThread>
#include <tiles/OverpassFilter.h>
#include "params.h"


class DynamicTilesGenerator : public QThread
{
    Q_OBJECT
public:
    //DynamicTilesGenerator(QString osmFile, QString textureFile, QString outPngDir, OverpassFilter *filter, int lat, int lon, int size, double frac, int prec);
    DynamicTilesGenerator(const QString &dynamicMapDir, int size, const QVector<QCheckBox *> &checkedDist);
signals:
    void tileGenerated();
public slots:
    void startPainting();
private:
    QString dynamicMapDir;

    QString textureFile;
    int size;
    double frac;
    int prec;
    double lat;
    double lon;

    QString svgFile = QString(STYLE_DIR) + "test.svg";
    QVector<QCheckBox*> checkedDist;

    // QThread interface
protected:
    void run();
};

#endif // DYNAMICTILESGENERATOR_H
