#include "mapscreen.h"
#include "ui_mapscreen.h"
#include <QSizePolicy>
#include "params.h"

MapScreen::MapScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapScreen)
{
    ui->setupUi(this);
    dynamicMapDir = DYNAMIC_MAP_DIR;
    dynamicMap = new DynamicMapGL(dynamicMapDir + "params.txt");
    vectorMap = new VectorMapGL;
    staticMap = new StaticMapGL;
    int h = ui->verticalLayoutWidget->height();
    vectorMap->setMinimumHeight(h);
    dynamicMap->setMinimumHeight(h);
    staticMap->setMinimumHeight(h);
    ui->verticalLayout->addWidget(dynamicMap);
    ui->verticalLayout->addWidget(vectorMap);
    ui->verticalLayout->addWidget(staticMap);

    dynamicMap->setFocusPolicy(Qt::StrongFocus);
}


void MapScreen::setMapDynamic(QVector<QString> checkedDist)
{
    vectorMap->hide();
    staticMap->hide();
    dynamicMapDir + "params.txt";
    this->checkedDist = checkedDist;
    currentRadius = 0;
    dynamicMap->setParams(checkedDist[currentRadius]);
    dynamicMap->show();
}

void MapScreen::setMapVector(QVector<QString> checkedDist)
{
    dynamicMap->hide();
    staticMap->hide();
    this->checkedDist = checkedDist;
    currentRadius = 0;
    vectorMap->setParams(checkedDist[currentRadius]);
    vectorMap->show();
}

void MapScreen::setMapStatic(QVector<QString> checkedDist)
{
    dynamicMap->hide();
    vectorMap->hide();
    this->checkedDist = checkedDist;
    currentRadius = 0;
    staticMap->setParams(checkedDist[currentRadius]);
    staticMap->show();
}

MapScreen::~MapScreen()
{
    delete ui;
}

void MapScreen::zoomIn(){
    if(currentRadius + 1 < checkedDist.size()){
        currentRadius++;
        if(!dynamicMap->isHidden())
            dynamicMap->setParams(checkedDist[currentRadius]);
        if(!vectorMap->isHidden())
            vectorMap->setParams(checkedDist[currentRadius]);
        if(!staticMap->isHidden())
            staticMap->setParams(checkedDist[currentRadius]);
    }
}

void MapScreen::zoomOut(){
    if(currentRadius - 1 >= 0){
        currentRadius--;
        if(!dynamicMap->isHidden())
            dynamicMap->setParams(checkedDist[currentRadius]);
        if(!vectorMap->isHidden())
            vectorMap->setParams(checkedDist[currentRadius]);
        if(!staticMap->isHidden())
            staticMap->setParams(checkedDist[currentRadius]);
    }
}
