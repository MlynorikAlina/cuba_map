#ifndef MAPSCREEN_H
#define MAPSCREEN_H

#include <QCheckBox>
#include <QResizeEvent>
#include <QWidget>
#include <dynamicmapgl.h>
#include <staticmapgl.h>
#include <vectormapgl.h>


class MapScreen : public QWidget
{
    Q_OBJECT

public:
    explicit MapScreen(QWidget *parent = nullptr);
    void setMapDynamic(QVector<QString> checkedDist);
    void setMapVector(QVector<QString> checkedDist);
    void setMapStatic(QVector<QString> checkedDist);
    ~MapScreen();

public slots:
    void zoomIn();
    void zoomOut();
private:
    QAtomicInt currentRadius;
    QVector<QString> checkedDist;
    QString staticMapDir;
    QString dynamicMapDir;
    DynamicMapGL* dynamicMap;
    VectorMapGL * vectorMap;
    StaticMapGL * staticMap;
};

#endif // MAPSCREEN_H
