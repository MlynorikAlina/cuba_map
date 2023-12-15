#ifndef DYNAMICSCREEN_H
#define DYNAMICSCREEN_H

#include <QPropertyAnimation>
#include <QWidget>
#include <parameterswindow.h>
#include <qnetworkaccessmanager.h>
#include <tilesselector.h>

#include <tiles/OverpassFilter.h>

namespace Ui {
class DynamicScreen;
}

class DynamicScreen : public QWidget
{
    Q_OBJECT

public:
    explicit DynamicScreen(QWidget *parent = nullptr);
    ~DynamicScreen();
    void setParametersWindow(ParametersWindow *newP);
private:
    Ui::DynamicScreen *ui;
    QAtomicInt filesToLoad;
    QAtomicInt filesToLoadTotal;
    QAtomicInt parsed;
    TilesSelector* tileSelector;
    ParametersWindow * p;
    Params * par;
    void parseFiles(QVector<int> borders);
    void generateTiles();
    Bbox b;
private slots:
    void loadDynamic(Params * p);
    void fileLoad();
    void startGeneratingTiles();

};

#endif // DYNAMICSCREEN_H
