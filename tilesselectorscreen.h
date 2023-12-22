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
    Selection getBox();
signals:
    void clicked();
private:
    Ui::TilesSelectorScreen *ui;
    QAtomicInt filesToLoad;
    QAtomicInt filesToLoadTotal;
    QAtomicInt parsed;
    TilesSelector* tileSelector;
};

#endif // TILESSELECTORSCREEN_H
