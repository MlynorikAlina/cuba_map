#include <osmparser.h>
#include "parsescreen.h"
#include "ui_parsescreen.h"


#include <QProcess>
#include <math.h>
#include <params.h>
#include <stdexcept>
#include <QThread>
#include <QMutex>
#include <QNetworkReply>
#include <networkdownload.h>
#include <asterdownloader.h>
#include <QDir>


#define OSM_SUFFIX "-latest.osm.pbf"

ParseScreen::ParseScreen(QWidget *parent)
    : QWidget(parent), ui(new Ui::ParseScreen) {
    ui->setupUi(this);
    ui->progressBar->setValue(0);
    ui->progressBar->hide();

    tileSelector = new TilesSelector;
    ui->verticalLayout->addWidget(tileSelector);
    tileSelector->setFocusPolicy(Qt::StrongFocus);

    connect(ui->parseButton, &QPushButton::clicked, this,
            &ParseScreen::parseFiles);
}

ParseScreen::~ParseScreen() { delete ui; }

void ParseScreen::parseFiles() {
    tileSelector->setEnabled(false);
    ui->parseButton->hide();
    ui->progressBar->show();
    Selection box = tileSelector->getBox();
    int minLat = std::min(box.coords[1], box.coords[7]);
    int maxLat = std::max(box.coords[1], box.coords[7]);
    int minLon = std::min(box.coords[0], box.coords[2]);
    int maxLon = std::max(box.coords[0], box.coords[2]);
    QVector<int> l = {minLon, maxLon, minLat, maxLat};

    QDir d(OSM_DIR);
    if(!d.exists())
        d.mkpath(d.absolutePath());

    d.setPath(ASTER_DIR);
    if(!d.exists())
        d.mkpath(d.absolutePath());

    filesToLoadTotal+=5*(l[3]-l[2])*(l[1]-l[0]);

    AsterDownloader* asterParser  = new AsterDownloader(ASTER_DIR, ASTER_URL);
    asterParser->setList(l);
    connect(asterParser, &AsterDownloader::fileDownloaded, this, &ParseScreen::fileLoad);

    OSMParser * p = new OSMParser(OSM_DIR, WORLD_PBF_DIR);
    connect(p, &OSMParser::fileParsed, this, &ParseScreen::fileLoad);
    p->setList(l);
    p->start();
    asterParser->run();
}

void ParseScreen::fileLoad()
{
    filesToLoad++;
    ui->progressBar->setRange(0, filesToLoadTotal);
    ui->progressBar->setValue(filesToLoad);
    if(filesToLoad == filesToLoadTotal){
        ui->progressBar->hide();
        ui->parseButton->show();
        tileSelector->setEnabled(true);
    }
}


