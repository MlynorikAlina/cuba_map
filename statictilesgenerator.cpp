#include <tiles/png_converter/SVGToPNGConverter.h>
#include "statictilesgenerator.h"
#include "params.h"
#include "tiles/GeoInfo.h"

#include <qfile.h>
#include <qprocess.h>
#include <settingsscreen.h>

#include <tiles/svg_converter/OSMToSVGConverter.h>



StaticTilesGenerator::StaticTilesGenerator(double lat, double lon, int size, const QVector<QString> &checkedDist, bool same):
    checkedDist(checkedDist), lat(lat), lon(lon), size(size), same(same)
{

}

void StaticTilesGenerator::load()
{
    start();
}

void StaticTilesGenerator::run()
{
    __TIME__
    for(auto dist:checkedDist){
        if(!same||!QFile::exists(STATIC_MAP_DIR + dist + ".png")){
            OverpassFilter *filter = SettingsScreen::getFilter(dist.toInt());
            double dlon = GeoInfo::km2long(dist.toInt(),lat);
            double dlat = GeoInfo::km2lat(dist.toInt());
            Bbox border;
            border.setBox(lat - dlat, lon - dlon, lat + dlat, lon + dlon);

            QStringList args;

            args <<STAT_ASTER_PARSER_PY << ASTER_DIR << TMP_STAT_TEXTURE <<QString::number(size);
            args<<QString::number(border.minLon,'f', 8)<<QString::number(border.minLat,'f', 8)<<QString::number(border.maxLon,'f', 8)<<QString::number(border.maxLat,'f', 8);
            QProcess* proc = new QProcess;
            proc->start(PYTHON, args);

            if (!proc->waitForStarted(-1) || !proc->waitForFinished(-1)) {
                return;
            }
            qDebug()<<proc->readAllStandardError();
            qDebug()<<proc->readAllStandardOutput();

            OSMToSVGConverter cv(TMP_STAT_OSM, TMP_STAT_TEXTURE, *filter, string(STYLE_DIR) + "test.svg", SettingsScreen::getStatStyle().toStdString(), border, size, size);
            cv.draw(PATTERNS_FILE, 1);
            SVGToPNGConverter png(size, size);
            png.convert(string(STYLE_DIR) + "test.svg", STATIC_MAP_DIR, dist.toStdString() + ".png");
        }
    }
    __TIME__
}
