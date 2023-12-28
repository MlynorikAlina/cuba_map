#include <tiles/png_converter/SVGToPNGConverter.h>
#include "statictilesgenerator.h"
#include "params.h"
#include "asterparser.h"
#include "tiles/GeoInfo.h"

#include <QDir>
#include <qfile.h>
#include <qprocess.h>
#include <settingsscreen.h>

#include <tiles/svg_converter/OSMToSVGConverter.h>



StaticTilesGenerator::StaticTilesGenerator(double lat, double lon, int size, const QVector<QString> &checkedDist, bool same, QString style):
    checkedDist(checkedDist), lat(lat), lon(lon), size(size), same(same), style(style)
{

}

void StaticTilesGenerator::load()
{
    start();
}

void StaticTilesGenerator::run()
{
    __TIME__
    if(!same){
        QDir dir(STATIC_MAP_DIR);
        dir.removeRecursively();
        dir.mkpath("./");
    }

    for(auto dist:checkedDist){
        if(!same||!QFile::exists(STATIC_MAP_DIR + dist + ".png")){
            OverpassFilter *filter = SettingsScreen::getFilter(dist.toInt());
            double dlon = GeoInfo::km2long(dist.toInt(),lat);
            double dlat = GeoInfo::km2lat(dist.toInt());
            Bbox border;
            border.setBox(lat - dlat, lon - dlon, lat + dlat, lon + dlon);

            AsterParser p;
            p.setStaticArgs(TMP_STAT_TEXTURE, ASTER_DIR, size, border);
            p.exec();

            OSMToSVGConverter cv(TMP_STAT_OSM, TMP_STAT_TEXTURE, *filter, string(STYLE_DIR) + "test.svg", style.toStdString(), border, size, size);
            cv.draw(PATTERNS_FILE, 1);
            SVGToPNGConverter png(size, size);
            png.convert(QString(STYLE_DIR) + "test.svg", STATIC_MAP_DIR, dist + ".png");
        }
    }
    __TIME__
}
