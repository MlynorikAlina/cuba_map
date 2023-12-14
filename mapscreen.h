#ifndef MAPSCREEN_H
#define MAPSCREEN_H

#include <QCheckBox>
#include <QResizeEvent>
#include <QWidget>
#include <dynamicmapgl.h>
#include <vectormapgl.h>

namespace Ui {
class MapScreen;
}

class MapScreen : public QWidget
{
    Q_OBJECT

public:
    explicit MapScreen(QWidget *parent = nullptr);
    void setMapDynamic(QVector<QCheckBox *> checkedDist);
    void setMapVector(QVector<QCheckBox *> checkedDist);
    ~MapScreen();

public slots:
    void zoomIn();
    void zoomOut();
private:
    QAtomicInt currentRadius;
    QVector<QCheckBox*> checkedDist;
    QString staticMapDir;
    QString dynamicMapDir;
    DynamicMapGL* dynamicMap;
    VectorMapGL * vectorMap;
    Ui::MapScreen *ui;
};

#endif // MAPSCREEN_H
