#include <osmtilesgenerator.h>
#include "tilesselectorscreen.h"
#include <params.h>
#include <QDir>
#include <QFormLayout>


TilesSelectorScreen::TilesSelectorScreen(QWidget *parent)
    : QWidget(parent) {
    tileSelector = new TilesSelector;
    parseButton = new QPushButton("parse", this);
    tileSelector->setParent(this);
    tileSelector->setGeometry(10, 10, W_WIDTH-20, W_HEIGHT - 60);
    parseButton->setGeometry(W_WIDTH-90, W_HEIGHT - 40, 80, 25);
    tileSelector->setFocusPolicy(Qt::StrongFocus);
    QDir dir(DYNAMIC_MAP_DIR);
    if(!dir.exists("./"))
        dir.mkpath("./");
    connect(parseButton, &QPushButton::clicked, this, &TilesSelectorScreen::hide);
    connect(parseButton, &QPushButton::clicked, this, &TilesSelectorScreen::clicked);
}

TilesSelectorScreen::~TilesSelectorScreen() { delete parseButton; delete tileSelector; }

Selection TilesSelectorScreen::getBox()
{
    return tileSelector->getBox();
}




