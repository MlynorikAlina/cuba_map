#ifndef MAPLOADER_H
#define MAPLOADER_H

#include "parameterswindow.h"
#include "tilesselectorscreen.h"



class MapLoader : public QObject
{
    Q_OBJECT
public:
    MapLoader(ParametersWindow *p, TilesSelectorScreen *tileSelector);

public slots:
    void loadVector(Params * par);
    void loadStatic(Params * par);
    void loadDynamic(Params * par);
private slots:
    void startGeneratingTiles();
private:
    ParametersWindow * p;
    Params * par;
    TilesSelectorScreen* tileSelector;
    Bbox b;
    QAtomicInt parsed;
    void generateDynamicTiles();
};

#endif // MAPLOADER_H
