#include "mainwindow.h"
#include "params.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <asterdownloader.h>
#include <cmath>
#include <vectorosmloader.h>
#include <vectortextureloader.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mapOptions = new QMenu("options");
    QAction* zoomIn = new QAction("zoom in");
    QAction* zoomOut = new QAction("zoom out");
    mapOptions->addAction(zoomIn);
    mapOptions->addAction(zoomOut);
    ui->menubar->addMenu(mapOptions);
    mapOptions->menuAction()->setVisible(false);

    connect(ui->actionsettings,&QAction::triggered, this, &MainWindow::showSettingsScreen);
    connect(ui->actiondynamic_map,&QAction::triggered, this, &MainWindow::showDynamicScreen);
    connect(ui->actionvector_map,&QAction::triggered, this, &MainWindow::showVectorScreen);

    connect(zoomIn, &QAction::triggered, ui->mapScreen, &MapScreen::zoomIn);
    connect(zoomOut, &QAction::triggered, ui->mapScreen, &MapScreen::zoomOut);

    p = new ParametersWindow(this);
    connect(p, &ParametersWindow::showDynamic, this,  &MainWindow::showDynamicMap);
    connect(p, &ParametersWindow::showVector, this,  &MainWindow::showVectorMap);
    connect(p, &ParametersWindow::parametersLoaded, this, &MainWindow::loadVector);

    ui->dynamicScreen->setParametersWindow(p);
    p->setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
    p->setWindowTitle(" ");
    showDynamicScreen();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showSettingsScreen()
{
    ui->settingsScreen->show();
    ui->dynamicScreen->hide();
    ui->mapScreen->hide();
    mapOptions->menuAction()->setVisible(false);
    p->hide();
}



void MainWindow::showDynamicScreen()
{
    p->setDynamicParams();
    ui->dynamicScreen->show();
    ui->dynamicScreen->setEnabled(true);
    ui->mapScreen->hide();
    ui->settingsScreen->hide();
    mapOptions->menuAction()->setVisible(false);
}

void MainWindow::showVectorScreen()
{
    p->show();
    p->setVectorParams();
    ui->dynamicScreen->hide();
    ui->mapScreen->hide();
    ui->settingsScreen->hide();
    mapOptions->menuAction()->setVisible(false);
}

void MainWindow::showDynamicMap(QVector<QString> checkedDist)
{
    mapOptions->menuAction()->setVisible(true);
    ui->mapScreen->show();
    ui->mapScreen->setMapDynamic(checkedDist);
    ui->dynamicScreen->hide();
    ui->settingsScreen->hide();
    p->hide();
}

void MainWindow::showVectorMap(QVector<QString> checkedDist)
{
    mapOptions->menuAction()->setVisible(true);
    ui->mapScreen->show();
    ui->mapScreen->setMapVector(checkedDist);
    ui->dynamicScreen->hide();
    ui->settingsScreen->hide();
    p->hide();
}

void MainWindow::loadVector(Params * par)
{
    if (!par->isDynamicParams && par->checkedDist.size() > 0) {

        __TIME__

        QFile f(VECTOR_MAP_PARAMS_FILE);
        bool same = false;
        if(f.exists()){
            f.open(QFile::ReadOnly);
            QTextStream ss(&f);
            QStringList sl = ss.readLine().split(" ");
            QString dist = ss.readLine();
            dist = ss.readLine();
            f.close();
            same = (sl[0] == par->c_lat && sl[1] == par->c_lon && fabs(dist.toDouble() - par->checkedDist[0].toDouble()) <= 1e-7);

        }
        if(!same && f.open(QFile::WriteOnly)){
            QTextStream ss(&f);
            ss<<par->c_lat<<" "<<par->c_lon<<Qt::endl;
            ss<<OSM_DIR<<endl;
            ss<<par->checkedDist[0];
            f.close();
        }

        p->setProgressRange(0,0);
        p->updProgress(-1);

        if(!same){

            VectorOsmLoader * ol = new VectorOsmLoader(par->c_lat.toDouble(), par->c_lon.toDouble(), par->checkedDist[0].toDouble());
            AsterDownloader* asterParser  = new AsterDownloader(ASTER_DIR, ASTER_URL);
            VectorTextureLoader * tl = new VectorTextureLoader(TMP_VEC_TEXTURE,ASTER_DIR, ol->getBox());
            connect(tl, &VectorTextureLoader::finished, ol, &VectorOsmLoader::load);
            connect(ol, &VectorOsmLoader::finished, p, &ParametersWindow::finishProgress);

            Bbox box =  ol->getBox();
            QVector<int> borders = {int(floor(box.minLon)), int(ceil(box.maxLon)), int(floor(box.minLat)), int(ceil(box.maxLat))};
            asterParser->setList(borders);
            connect(asterParser, &AsterDownloader::finished, tl, &VectorTextureLoader::load);

            asterParser->run();
        }
        else p->finishProgress();
    }
}


