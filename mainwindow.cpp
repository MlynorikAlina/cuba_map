#include "mainwindow.h"
#include "asterparser.h"
#include "params.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <asterdownloader.h>
#include <cmath>
#include <osmloader.h>
#include <statictilesgenerator.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    mapOptions = new QMenu("options");
    QAction *zoomIn = new QAction("zoom in");
    QAction *zoomOut = new QAction("zoom out");
    mapOptions->addAction(zoomIn);
    mapOptions->addAction(zoomOut);
    ui->menubar->addMenu(mapOptions);
    mapOptions->menuAction()->setVisible(false);
    mapLoader = new MapLoader(ui->parametersScreen, ui->tilesSelectorScreen);

    connect(ui->actionsettings, &QAction::triggered, this,
            &MainWindow::showSettingsScreen);
    connect(ui->actiondynamic_map, &QAction::triggered, this,
            &MainWindow::showDynamicScreen);
    connect(ui->actionvector_map, &QAction::triggered, this,
            &MainWindow::showVectorScreen);
    connect(ui->actionstatic_map, &QAction::triggered, this,
            &MainWindow::showStaticScreen);

    connect(zoomIn, &QAction::triggered, ui->mapScreen, &MapScreen::zoomIn);
    connect(zoomOut, &QAction::triggered, ui->mapScreen, &MapScreen::zoomOut);

    connect(ui->parametersScreen, &ParametersWindow::showDynamic, this, &MainWindow::showDynamicMap);
    connect(ui->parametersScreen, &ParametersWindow::showVector, this, &MainWindow::showVectorMap);
    connect(ui->parametersScreen, &ParametersWindow::showStatic, this, &MainWindow::showStaticMap);

    connect(ui->parametersScreen, &ParametersWindow::parametersLoaded, mapLoader, &MapLoader::loadDynamic);
    connect(ui->parametersScreen, &ParametersWindow::parametersLoaded, mapLoader,
            &MapLoader::loadVector);
    connect(ui->parametersScreen, &ParametersWindow::parametersLoaded, mapLoader,
            &MapLoader::loadStatic);

    ui->tilesSelectorScreen->connectParametersWindow(ui->parametersScreen);
    showDynamicScreen();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::showSettingsScreen() {
    ui->settingsScreen->show();
    ui->tilesSelectorScreen->hide();
    ui->mapScreen->hide();
    mapOptions->menuAction()->setVisible(false);
    ui->parametersScreen->hide();
}

void MainWindow::showDynamicScreen() {
    ui->parametersScreen->hide();
    ui->tilesSelectorScreen->show();
    ui->mapScreen->hide();
    ui->settingsScreen->hide();
    mapOptions->menuAction()->setVisible(false);
}

void MainWindow::showVectorScreen() {
    ui->parametersScreen->show();
    ui->parametersScreen->setVectorParams();
    ui->tilesSelectorScreen->hide();
    ui->mapScreen->hide();
    ui->settingsScreen->hide();
    mapOptions->menuAction()->setVisible(false);
}
void MainWindow::showStaticScreen() {
    ui->parametersScreen->show();
    ui->parametersScreen->setStaticParams();
    ui->tilesSelectorScreen->hide();
    ui->mapScreen->hide();
    ui->settingsScreen->hide();
    mapOptions->menuAction()->setVisible(false);
}

void MainWindow::showDynamicMap(QVector<QString> checkedDist) {
    mapOptions->menuAction()->setVisible(true);
    ui->mapScreen->show();
    ui->mapScreen->setMapDynamic(checkedDist);
    ui->tilesSelectorScreen->hide();
    ui->settingsScreen->hide();
    ui->parametersScreen->hide();
}

void MainWindow::showVectorMap(QVector<QString> checkedDist) {
    mapOptions->menuAction()->setVisible(true);
    ui->mapScreen->show();
    ui->mapScreen->setMapVector(checkedDist);
    ui->tilesSelectorScreen->hide();
    ui->settingsScreen->hide();
    ui->parametersScreen->hide();
}

void MainWindow::showStaticMap(QVector<QString> checkedDist) {
    mapOptions->menuAction()->setVisible(true);
    ui->mapScreen->show();
    ui->mapScreen->setMapStatic(checkedDist);
    ui->tilesSelectorScreen->hide();
    ui->settingsScreen->hide();
    ui->parametersScreen->hide();
}


