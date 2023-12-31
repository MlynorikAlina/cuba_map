#include <osmtilesgenerator.h>
#include "tilesselectorscreen.h"
#include "ui_tilesselectorscreen.h"
#include <params.h>
#include <QDir>


TilesSelectorScreen::TilesSelectorScreen(QWidget *parent)
    : QWidget(parent), ui(new Ui::TilesSelectorScreen) {
    ui->setupUi(this);

    tileSelector = new TilesSelector;
    ui->verticalLayout->addWidget(tileSelector);
    tileSelector->setFocusPolicy(Qt::StrongFocus);
    QDir dir(DYNAMIC_MAP_DIR);
    if(!dir.exists())
        dir.mkpath("./");
    connect(ui->parseButton, &QPushButton::clicked, this, &TilesSelectorScreen::hide);
    connect(ui->parseButton, &QPushButton::clicked, this, &TilesSelectorScreen::clicked);
}

TilesSelectorScreen::~TilesSelectorScreen() { delete ui; }

Selection TilesSelectorScreen::getBox()
{
    return tileSelector->getBox();
}




