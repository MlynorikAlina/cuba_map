#ifndef TILESSELECTORSCREEN_H
#define TILESSELECTORSCREEN_H

#include "qpushbutton.h"
#include <QPropertyAnimation>
#include <QWidget>
#include <tilesselector.h>

#include <tiles/OverpassFilter.h>


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
    QPushButton* parseButton;
    TilesSelector* tileSelector;
};

#endif // TILESSELECTORSCREEN_H
