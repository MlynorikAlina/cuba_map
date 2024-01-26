#include "vectorosmloader.h"
#include "params.h"
#include "qdebug.h"
#include "tiles/GeoInfo.h"

#include <cmath>
#include <qprocess.h>
#include <qtextstream.h>

VectorOsmLoader::VectorOsmLoader(double clat, double clon, double max_dist)
{
    double dlat = GeoInfo::km2long(max_dist,clat);
    double dlon = GeoInfo::km2lat(max_dist);
    b.setBox(clat - dlat, clon - dlon, clat + dlat, clon + dlon);
}

void VectorOsmLoader::load()
{
    start();
}

void VectorOsmLoader::run()
{
    __TIME__
    QString box = "-b=";
    QTextStream ss(&box);
    ss.setRealNumberNotation(QTextStream::FixedNotation);

    ss<<b.minLon<<","<<b.minLat<<","<<b.maxLon<<","<<b.maxLat;
    QStringList args;
    for(int i = floor((b.minLat)/PLANET_TILES_STEP)*PLANET_TILES_STEP; i<=floor((b.maxLat)/PLANET_TILES_STEP)*PLANET_TILES_STEP;i+=PLANET_TILES_STEP)
        for(int j = floor((b.minLon)/PLANET_TILES_STEP)*PLANET_TILES_STEP; j<=floor((b.maxLon)/PLANET_TILES_STEP)*PLANET_TILES_STEP;j+=PLANET_TILES_STEP)
            args<<WORLD_PBF_DIR + QString::number(i + 90) + "_" + QString::number(j + 180) + ".osm.pbf";

    args<<box<<"--drop-author" << "--drop-version"<<"--complete-ways"<<"--drop-broken-refs"<<QString("-o=") + TMP_VEC_OSM;


    QProcess proc;
    proc.start(OSMCONVERT, args);
    if (!proc.waitForStarted(-1) || !proc.waitForFinished(-1)) {
        return ;
    }
    __TIME__
}

Bbox VectorOsmLoader::getBox() const
{
    return b;
}
