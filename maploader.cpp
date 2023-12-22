#include "asterdownloader.h"
#include "asterparser.h"
#include "dynamictilesgenerator.h"
#include "maploader.h"
#include "osmloader.h"
#include "osmtilesgenerator.h"
#include "params.h"
#include "settingsscreen.h"
#include "statictilesgenerator.h"
#include "tilesselector.h"

#include <QDir>
#include <QFile>
#include <cmath>


MapLoader::MapLoader(ParametersWindow *p, TilesSelectorScreen *tileSelector) : p(p),
    tileSelector(tileSelector)
{}

void MapLoader::loadVector(Params *par)
{
    if (par->mode == VECTOR && par->checkedDist.size() > 0) {

        __TIME__
        QFile f(VECTOR_MAP_PARAMS_FILE);
        bool same = false;
        if (f.exists()) {
            f.open(QFile::ReadOnly);
            QTextStream ss(&f);
            QStringList sl = ss.readLine().split(" ");
            QString dist = ss.readLine();
            dist = ss.readLine();
            f.close();
            same = (sl[0] == par->c_lat && sl[1] == par->c_lon &&
                    fabs(dist.toDouble() - par->checkedDist[0].toDouble()) <= 1e-7);
        }
        if (!same && f.open(QFile::WriteOnly)) {
            QTextStream ss(&f);
            ss << par->c_lat << " " << par->c_lon << Qt::endl;
            ss << OSM_DIR << Qt::endl;
            ss << par->checkedDist[0];
            f.close();
        }

        p->setProgressRange(0, 0);
        p->updProgress(-1);

        if (!same) {
            OSMLoader *ol =
                new OSMLoader(par->c_lat.toDouble(), par->c_lon.toDouble(),
                              par->checkedDist[0].toDouble());
            AsterDownloader *ad = new AsterDownloader(ASTER_DIR, ASTER_URL);
            AsterParser *ap = new AsterParser;
            ap->setVectorArgs(TMP_VEC_TEXTURE, ASTER_DIR, ol->getBox());
            connect(ap, &AsterParser::finished, ol, &OSMLoader::load);
            connect(ol, &OSMLoader::finished, p, &ParametersWindow::finishProgress);

            Bbox box = ol->getBox();
            QVector<int> borders = {int(floor(box.minLon)), int(ceil(box.maxLon)),
                                    int(floor(box.minLat)), int(ceil(box.maxLat))};
            ad->setList(borders);
            connect(ad, &AsterDownloader::finished, ap, &AsterParser::load);
            ad->run();
        } else
            p->finishProgress();
    }
}

void MapLoader::loadStatic(Params *par)
{
    if (par->mode == STATIC && par->checkedDist.size() > 0) {

        __TIME__
        p->setProgressRange(0, 0);
        p->updProgress(-1);

        QFile f(STATIC_MAP_PARAMS_FILE);
        bool same = false;
        QString prevStyle;
        QString style = SettingsScreen::getStatStyle();
        if (f.exists()) {
            f.open(QFile::ReadOnly);
            QTextStream ss(&f);
            QStringList sl = ss.readLine().split(" ");
            prevStyle = ss.readLine();
            QString dist = ss.readLine();
            dist = ss.readLine();
            f.close();
            same = (sl[0] == par->c_lat && sl[1] == par->c_lon &&
                    (dist == par->checkedDist[0] ||
                     dist.toDouble() > par->checkedDist[0].toDouble()));
        }
        bool tilesSame = same && (SettingsScreen::getStatStyle() == prevStyle);
        if (!tilesSame && f.open(QFile::WriteOnly)) {
            QTextStream ss(&f);
            ss << par->c_lat << " " << par->c_lon << Qt::endl;
            ss << style << Qt::endl;
            ss << OSM_DIR << Qt::endl;
            ss << par->checkedDist[0];
            f.close();
        }

        OSMLoader *ol = new OSMLoader(par->c_lat.toDouble(), par->c_lon.toDouble(),
                                      par->checkedDist[0].toDouble());
        AsterDownloader *asterParser = new AsterDownloader(ASTER_DIR, ASTER_URL);

        StaticTilesGenerator *tg = new StaticTilesGenerator(
            par->c_lat.toDouble(), par->c_lon.toDouble(), STATIC_TILE_SIZE,
            par->checkedDist, tilesSame, style);

        connect(tg, &StaticTilesGenerator::finished, p,
                &ParametersWindow::finishProgress);

        Bbox box = ol->getBox();
        QVector<int> borders = {int(floor(box.minLon)), int(ceil(box.maxLon)),
                                int(floor(box.minLat)), int(ceil(box.maxLat))};
        asterParser->setList(borders);
        if (!same) {
            connect(asterParser, &AsterDownloader::finished, ol, &OSMLoader::load);
            connect(ol, &OSMLoader::finished, tg, &StaticTilesGenerator::load);
        } else {
            connect(asterParser, &AsterDownloader::finished, tg,
                    &StaticTilesGenerator::load);
        }
        asterParser->run();
    }
}

void MapLoader::loadDynamic(Params *par)
{
    if(par->mode == DYNAMIC){
        this->par = par;
        Selection box = tileSelector->getBox();
        int minLat = std::min(box.coords[1], box.coords[7]);
        int maxLat = std::max(box.coords[1], box.coords[7]);
        int minLon = std::min(box.coords[0], box.coords[2]);
        int maxLon = std::max(box.coords[0], box.coords[2]);
        b.setBox(minLat,minLon, maxLat, maxLon);

        QVector<int> l = {minLon, maxLon, minLat, maxLat};

        int osmCount=(l[3]-l[2])*(l[1]-l[0]);
        int filesToLoadTotal = (1+(par->checkedDist.size() + 1)/(TILE_STEP*TILE_STEP)) * osmCount;
        parsed=0;
        qDebug()<<"total:"<<filesToLoadTotal;
        p->setProgressRange(0,filesToLoadTotal);

        QDir d(OSM_DIR);
        if(!d.exists())
            d.mkpath(d.absolutePath());

        d.setPath(ASTER_DIR);
        if(!d.exists())
            d.mkpath(d.absolutePath());

        AsterDownloader* asterParser  = new AsterDownloader(ASTER_DIR, ASTER_URL);
        asterParser->setList(l);
        connect(asterParser, SIGNAL(fileDownloaded()), p, SLOT(updProgress()));
        connect(asterParser, &AsterDownloader::finished, this, &MapLoader::startGeneratingTiles);

        OSMTilesGenerator * tg = new OSMTilesGenerator(OSM_DIR, WORLD_PBF_DIR);
        connect(tg, SIGNAL(fileParsed()), p, SLOT(updProgress()));
        connect(tg, &OSMTilesGenerator::finished, this, &MapLoader::startGeneratingTiles);
        tg->setList(l);
        tg->start();
        asterParser->run();
    }
}

void MapLoader::generateDynamicTiles() {
    if (par->checkedDist.size() > 0) {

        QFile f(QString(DYNAMIC_MAP_DIR) + "params.txt");
        if(f.open(QFile::WriteOnly)){
            QTextStream ss(&f);
            ss<<par->c_lat<<" "<<par->c_lon<<Qt::endl;
            QString tilesDir = DYNAMIC_MAP_DIR;
            ss<<tilesDir<<Qt::endl;
            f.close();
        }

        DynamicTilesGenerator* tg = new DynamicTilesGenerator(DYNAMIC_MAP_DIR, DEFAULT_TILE_SIZE, SettingsScreen::getStatStyle(), par->checkedDist);
        connect(tg, SIGNAL(tileGenerated()), p, SLOT(updProgress()));
        tg->setBorder(b);
        tg->start();
    }
}

void MapLoader::startGeneratingTiles()
{
    parsed++;
    if(parsed==2){
        generateDynamicTiles();
    }
}

