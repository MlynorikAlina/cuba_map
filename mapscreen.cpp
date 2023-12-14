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
    vectorMap = new VectorMapGL();
    int h = ui->verticalLayoutWidget->height();
    vectorMap->setMinimumHeight(h);
    dynamicMap->setMinimumHeight(h);
    ui->verticalLayout->addWidget(dynamicMap);
    ui->verticalLayout->addWidget(vectorMap);

    dynamicMap->setFocusPolicy(Qt::StrongFocus);
}


void MapScreen::setMapDynamic(QVector<QCheckBox *> checkedDist)
{
    vectorMap->hide();
    dynamicMapDir + "params.txt";
    this->checkedDist = checkedDist;
    currentRadius = 0;
    dynamicMap->setParams(checkedDist[currentRadius]->text());
    dynamicMap->show();
}

void MapScreen::setMapVector(QVector<QCheckBox *> checkedDist)
{
    dynamicMap->hide();
    this->checkedDist = checkedDist;
    currentRadius = 0;
    vectorMap->setParams(checkedDist[currentRadius]->text());
    vectorMap->show();
}

MapScreen::~MapScreen()
{
    delete ui;
}

void MapScreen::zoomIn(){
    if(currentRadius + 1 < checkedDist.size()){
        currentRadius++;
        if(!dynamicMap->isHidden())
            dynamicMap->setParams(checkedDist[currentRadius]->text());
        if(!vectorMap->isHidden())
            vectorMap->setParams(checkedDist[currentRadius]->text());
    }
}

void MapScreen::zoomOut(){
    if(currentRadius - 1 >= 0){
        currentRadius--;
        if(!dynamicMap->isHidden())
            dynamicMap->setParams(checkedDist[currentRadius]->text());
        if(!vectorMap->isHidden())
            vectorMap->setParams(checkedDist[currentRadius]->text());
    }
}
