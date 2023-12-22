#include <osmtilesgenerator.h>
#include "tilesselectorscreen.h"
#include "settingsscreen.h"
#include "ui_tilesselectorscreen.h"


#include <QProcess>
#include <math.h>
#include <params.h>
#include <stdexcept>
#include <QThread>
#include <QMutex>
#include <QNetworkReply>
#include <network.h>
#include <asterdownloader.h>
#include <QDir>
#include <dynamictilesgenerator.h>


#define OSM_SUFFIX "-latest.osm.pbf"

TilesSelectorScreen::TilesSelectorScreen(QWidget *parent)
    : QWidget(parent), ui(new Ui::TilesSelectorScreen) {
    ui->setupUi(this);

    tileSelector = new TilesSelector;
    ui->verticalLayout->addWidget(tileSelector);
    tileSelector->setFocusPolicy(Qt::StrongFocus);
    QDir dir(DYNAMIC_MAP_DIR);
    if(!dir.exists())
        dir.mkpath(DYNAMIC_MAP_DIR);
    connect(ui->parseButton, &QPushButton::clicked, this, &TilesSelectorScreen::hide);
    connect(ui->parseButton, &QPushButton::clicked, this, &TilesSelectorScreen::clicked);
}

TilesSelectorScreen::~TilesSelectorScreen() { delete ui; }

Selection TilesSelectorScreen::getBox()
{
    return tileSelector->getBox();
}




