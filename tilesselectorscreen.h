#ifndef TILESSELECTORSCREEN_H
#define TILESSELECTORSCREEN_H

#include <QPropertyAnimation>
#include <QWidget>
#include <parameterswindow.h>
#include <qnetworkaccessmanager.h>
#include <tilesselector.h>

#include <tiles/OverpassFilter.h>

namespace Ui {
class TilesSelectorScreen;
}

class TilesSelectorScreen : public QWidget
{
    Q_OBJECT

public:
    explicit TilesSelectorScreen(QWidget *parent = nullptr);
    ~TilesSelectorScreen();
    void connectParametersWindow(ParametersWindow *newP);
    Selection getBox();
private:
    Ui::TilesSelectorScreen *ui;
    ParametersWindow * p;
    QAtomicInt filesToLoad;
    QAtomicInt filesToLoadTotal;
    QAtomicInt parsed;
    TilesSelector* tileSelector;
private slots:
    void showParameters();
};

#endif // TILESSELECTORSCREEN_H
