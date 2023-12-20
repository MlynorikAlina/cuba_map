#include <tiles/png_converter/SVGToPNGConverter.h>
#include "staticosmloader.h"
#include "tiles/GeoInfo.h"
#include "params.h"
#include <qprocess.h>
#include <cmath>
#include <qtextstream.h>
#include <settingsscreen.h>
#include <tiles/svg_converter/OSMToSVGConverter.h>

StaticOsmLoader::StaticOsmLoader(double clat, double clon, double max_dist)
{
    double dlon = GeoInfo::km2long(max_dist,clat);
    double dlat = GeoInfo::km2lat(max_dist);
    b.setBox(clat - 1.5*dlat, clon - 1.5*dlon, clat + 1.5*dlat, clon + 1.5*dlon);
}

void StaticOsmLoader::load()
{
    start();
}

void StaticOsmLoader::run()
{
    __TIME__
    QString box = "-b=";
    QTextStream ss(&box);
    ss.setRealNumberNotation(QTextStream::FixedNotation);

    ss<<b.minLon<<","<<b.minLat<<","<<b.maxLon<<","<<b.maxLat;
    QStringList args;
    for(int i = floor((b.minLat)/PLANET_TILES_STEP)*PLANET_TILES_STEP; i<=floor((b.maxLat)/PLANET_TILES_STEP)*PLANET_TILES_STEP;i+=PLANET_TILES_STEP)
        for(int j = floor((b.minLon)/PLANET_TILES_STEP)*PLANET_TILES_STEP; j<=floor((b.maxLon)/PLANET_TILES_STEP)*PLANET_TILES_STEP;j+=PLANET_TILES_STEP)
            args<<PBF_DIR + QString::number(i + 90) + "_" + QString::number(j + 180) + ".osm.pbf";

    args<<box<<"--drop-author" << "--drop-version"<<"--drop-broken-refs"<<QString("-o=") + TMP_STAT_OSM;


    QProcess proc;
    proc.start("osmconvert", args);
    if (!proc.waitForStarted(-1) || !proc.waitForFinished(-1)) {
        return ;
    }
    __TIME__
}

Bbox StaticOsmLoader::getBox()const
{
    return b;
}
