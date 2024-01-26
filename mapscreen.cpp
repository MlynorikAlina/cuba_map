#include "mapscreen.h"
#include <QSizePolicy>
#include "params.h"

MapScreen::MapScreen(QWidget *parent) :
    QWidget(parent)
{
    dynamicMapDir = DYNAMIC_MAP_DIR;
    dynamicMap = new DynamicMapGL(dynamicMapDir + "params.txt", this);
    vectorMap = new VectorMapGL(this);
    staticMap = new StaticMapGL(this);
    vectorMap->resize(QSize(W_WIDTH,W_HEIGHT));
    dynamicMap->resize(QSize(W_WIDTH,W_HEIGHT));
    staticMap->resize(QSize(W_WIDTH,W_HEIGHT));
    //staticMap->setMaximumSize(QSize(w+1,h+1));
    dynamicMap->setFocusPolicy(Qt::StrongFocus);
    qInfo()<<__PRETTY_FUNCTION__;
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
    vectorMap->show();
    vectorMap->setParams(checkedDist[currentRadius]);
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
    delete dynamicMap;
    delete staticMap;
    delete vectorMap;
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
