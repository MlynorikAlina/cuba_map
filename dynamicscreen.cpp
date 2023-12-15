#include <osmparser.h>
#include "dynamicscreen.h"
#include "ui_dynamicscreen.h"


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
#include <dynamictilesgenerator.h>


#define OSM_SUFFIX "-latest.osm.pbf"

DynamicScreen::DynamicScreen(QWidget *parent)
    : QWidget(parent), ui(new Ui::DynamicScreen) {
    ui->setupUi(this);

    tileSelector = new TilesSelector;
    ui->verticalLayout->addWidget(tileSelector);
    tileSelector->setFocusPolicy(Qt::StrongFocus);
    QDir dir(DYNAMIC_MAP_DIR);
    if(!dir.exists())
        dir.mkpath(DYNAMIC_MAP_DIR);
}

DynamicScreen::~DynamicScreen() { delete ui; }

void DynamicScreen::setParametersWindow(ParametersWindow *newP)
{
    p = newP;
    connect(ui->parseButton, &QPushButton::clicked, p,
            &ParametersWindow::show);
    connect(p, &ParametersWindow::parametersLoaded, this, &DynamicScreen::loadDynamic);
}

void DynamicScreen::parseFiles(QVector<int> borders) {
    __TIME__
    QDir d(OSM_DIR);
    if(!d.exists())
        d.mkpath(d.absolutePath());

    d.setPath(ASTER_DIR);
    if(!d.exists())
        d.mkpath(d.absolutePath());



    AsterDownloader* asterParser  = new AsterDownloader(ASTER_DIR, ASTER_URL);
    asterParser->setList(borders);
    connect(asterParser, &AsterDownloader::fileDownloaded, this, &DynamicScreen::fileLoad);
    connect(asterParser, &AsterDownloader::finished, this, &DynamicScreen::startGeneratingTiles);

    OSMParser * p = new OSMParser(OSM_DIR, WORLD_PBF_DIR);
    connect(p, &OSMParser::fileParsed, this, &DynamicScreen::fileLoad);
    connect(p, &OSMParser::finished, this, &DynamicScreen::startGeneratingTiles);
    p->setList(borders);
    p->start();
    asterParser->run();
}

void DynamicScreen::loadDynamic(Params * par)
{
    if(par->isDynamicParams){
        setEnabled(false);
        this->par = par;
        Selection box = tileSelector->getBox();
        int minLat = std::min(box.coords[1], box.coords[7]);
        int maxLat = std::max(box.coords[1], box.coords[7]);
        int minLon = std::min(box.coords[0], box.coords[2]);
        int maxLon = std::max(box.coords[0], box.coords[2]);
        b.setBox(minLat,minLon, maxLat, maxLon);

        QVector<int> l = {minLon, maxLon, minLat, maxLat};

        int osmCount=(l[3]-l[2])*(l[1]-l[0]);
        filesToLoadTotal = (1+(par->checkedDist.size() + 1)/(TILE_STEP*TILE_STEP)) * osmCount;
        filesToLoad = 0;
        qDebug()<<"total:"<<filesToLoadTotal;
        p->setProgressRange(0,filesToLoadTotal);
        parseFiles(l);
    }
}

void DynamicScreen::generateTiles() {
    if (par->checkedDist.size() > 0) {

        QFile f(QString(DYNAMIC_MAP_DIR) + "params.txt");
        if(f.open(QFile::WriteOnly)){
            QTextStream ss(&f);
            ss<<par->c_lat<<" "<<par->c_lon<<Qt::endl;
            QString tilesDir = DYNAMIC_MAP_DIR;
            ss<<tilesDir;
            f.close();
        }

        DynamicTilesGenerator* tg = new DynamicTilesGenerator(DYNAMIC_MAP_DIR, DEFAULT_TILE_SIZE, par->checkedDist);
        connect(tg, &DynamicTilesGenerator::tileGenerated, this, &DynamicScreen::fileLoad);
        tg->setBorder(b);
        tg->start();
    }
}

void DynamicScreen::fileLoad()
{
    p->updProgress();
    //qDebug()<<++filesToLoad;
}

void DynamicScreen::startGeneratingTiles()
{
    parsed++;
    if(parsed==2){
        generateTiles();
    }
}

