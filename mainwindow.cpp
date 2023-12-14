#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>

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

    showParseScreen();
    connect(ui->actionparse_osm,&QAction::triggered, this, &MainWindow::showParseScreen);
    connect(ui->actionsettings,&QAction::triggered, this, &MainWindow::showSettingsScreen);
    connect(ui->actionset_map_options,&QAction::triggered, this, &MainWindow::showMapOptionsScreen);
    connect(ui->mapOptionsScreen, &MapOptionsScreen::showDynamic, this, &MainWindow::showDynamicMap);
    connect(ui->mapOptionsScreen, &MapOptionsScreen::showVector, this, &MainWindow::showVectorMap);
    connect(zoomIn, &QAction::triggered, ui->mapScreen, &MapScreen::zoomIn);
    connect(zoomOut, &QAction::triggered, ui->mapScreen, &MapScreen::zoomOut);

    this->setEnabled(false);
    p = new ParametersWindow;
    p->setWindowFlags(Qt::SubWindow|Qt::WindowStaysOnTopHint);
    p->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showSettingsScreen()
{
    ui->settingsScreen->show();
    ui->mapOptionsScreen->hide();
    ui->parseScreen->hide();
    ui->mapScreen->hide();
    mapOptions->menuAction()->setVisible(false);
}



void MainWindow::showParseScreen()
{
    ui->parseScreen->show();
    ui->mapOptionsScreen->hide();
    ui->mapScreen->hide();
    ui->settingsScreen->hide();
    mapOptions->menuAction()->setVisible(false);
}

void MainWindow::showMapOptionsScreen()
{
    ui->mapOptionsScreen->show();
    ui->parseScreen->hide();
    ui->mapScreen->hide();
    ui->settingsScreen->hide();
    mapOptions->menuAction()->setVisible(false);
}

void MainWindow::showDynamicMap(QVector<QCheckBox *> checkedDist)
{
    mapOptions->menuAction()->setVisible(true);
    ui->mapScreen->show();
    ui->mapScreen->setMapDynamic(checkedDist);
    ui->mapOptionsScreen->hide();
    ui->parseScreen->hide();
    ui->settingsScreen->hide();
}

void MainWindow::showVectorMap(QVector<QCheckBox *> checkedDist)
{
    mapOptions->menuAction()->setVisible(true);
    ui->mapScreen->show();
    ui->mapScreen->setMapVector(checkedDist);
    ui->mapOptionsScreen->hide();
    ui->parseScreen->hide();
    ui->settingsScreen->hide();
}



