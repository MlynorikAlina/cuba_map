#ifndef MAPOPTIONSSCREEN_H
#define MAPOPTIONSSCREEN_H

#include <QWidget>
#include <QFileSystemModel>
#include <QCheckBox>
#include <QSet>
#include <QHash>

#include <tiles/OverpassFilter.h>

namespace Ui {
class MapOptionsScreen;
}

enum UpdateMode {
    NO_UPDATE, UPDATE_FILTER, UPDATE_ALL
};

class MapOptionsScreen : public QWidget
{
    Q_OBJECT

public:
    explicit MapOptionsScreen(QWidget *parent = nullptr);
    void load();
    ~MapOptionsScreen();
    static UpdateMode updateMapParams(QString tempOut, OverpassFilter *filter, QString lat, QString lon);
signals:
    void showSatic(QVector<QCheckBox*> checkedDist);
    void showDynamic(QVector<QCheckBox*> checkedDist);
    void showVector(QVector<QCheckBox*> checkedDist);
public slots:
    void msg();
private slots:
    void showDynamicRadius();
    void showVectorRadius();
private:
    QAtomicInt counter;
    QString dynamicMapDir;
    QFileSystemModel *model;
    Ui::MapOptionsScreen *ui;
    QString dataFile = "parameters.txt";
    QVector<QCheckBox*> checkedDist;
    //void loadStatic(QString _osmDir);
    void loadDynamic(QString _osmDir);
    void loadVector(QString _osmDir);
};


#endif // MAPOPTIONSSCREEN_H
