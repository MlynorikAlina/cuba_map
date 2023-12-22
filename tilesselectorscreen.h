#ifndef TILESSELECTORSCREEN_H
#define TILESSELECTORSCREEN_H

#include <QPropertyAnimation>
#include <QWidget>
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
    TilesSelector* tileSelector;
};

#endif // TILESSELECTORSCREEN_H
