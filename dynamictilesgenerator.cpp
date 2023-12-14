#include <math.h>
#include <tiles/png_converter/SVGToPNGConverter.h>
#include "dynamictilesgenerator.h"
#include "mapoptionsscreen.h"
#include "qdebug.h"

#include "params.h"
#include <tiles/svg_converter/OSMToSVGConverter.h>

#include <QDirIterator>
#include <QTextStream>
#include <asterparserdynamic.h>
#include <settingsscreen.h>




/*DynamicTilesGenerator::DynamicTilesGenerator(QString osmFile, QString textureFile, QString outPngDir, OverpassFilter *filter, int lat, int lon, int size, double frac, int prec):
    osmFile(osmFile), textureFile(textureFile), outPngDir(outPngDir), filter(filter), size(size), frac(frac), prec(prec), lat(lat), lon(lon)
{

}*/

void DynamicTilesGenerator::startPainting()
{
    this->start();
}

void DynamicTilesGenerator::run()
{
    for (auto e : checkedDist) {
        OverpassFilter *filter = SettingsScreen::getFilter(e->text().toInt());

        UpdateMode mode = MapOptionsScreen::updateMapParams(dynamicMapDir+e->text()+"_p.txt", filter, "0","0");

        QDirIterator* it = new QDirIterator(OSM_DIR, QStringList("*.osm"),
                              QDir::Files | QDir::NoDotAndDotDot,
                              QDirIterator::Subdirectories);


        double frac = min(TILE_STEP, dynamicTilesSize[e->text()].toDouble());
        int tsize = round(size*TILE_STEP/frac);
        QString tempAster = PARSED_ASTER + e->text() + "/";
        QDir dir(tempAster);
        if (!dir.exists())
            dir.mkpath(dir.absolutePath());

        AsterParserDynamic *p = new AsterParserDynamic(ASTER_DIR, tempAster, QString::number(tsize), QString::number(TILE_STEP), QString::number(TILE_STEP_PREC));
        p->exec();

        QString outDir = dynamicMapDir + e->text() + "/";
        dir.setPath(outDir);
        while (it->hasNext()) {
            it->next();

            if (!dir.exists())
                dir.mkpath(dir.absolutePath());

            dir.setPath(it->filePath());
            dir.setPath(QDir::cleanPath(dir.filePath("../..")));

            QString coords = it->filePath();
            coords.remove(".osm");
            coords.remove(dir.path());
            coords.remove(0, 1);
            coords.replace("/", "_");
            QStringList sl = coords.split("_");
            coords = tempAster + coords + ".json";

            lat = sl[0].toDouble();
            lon = sl[1].toDouble();
            prec = max(dynamicTilesPrec[e->text()],TILE_STEP_PREC);

            OSMToSVGConverter* cv = NULL;

            SVGToPNGConverter png(size, size);
            png.setViewport(tsize);
            for (int shifty = 0; shifty < tsize; shifty += size){
                QString pngDir = outDir;
                QTextStream ss(&pngDir);
                ss.setRealNumberPrecision(prec);
                ss.setRealNumberNotation(QTextStream::FixedNotation);
                ss << lat + 1 - 1. * shifty / tsize - frac << '/';
                for (int shiftx = 0; shiftx < tsize; shiftx += size) {
                    QString pngFile;
                    QTextStream ssn(&pngFile);
                    ssn.setRealNumberPrecision(prec);
                    ssn.setRealNumberNotation(QTextStream::FixedNotation);
                    ssn<< lon +  1. * shiftx / tsize << ".png";
                    if(!QFile::exists(pngDir+pngFile) || mode!=NO_UPDATE){
                        if(cv==NULL){
                            Bbox b;
                            b.setBox(lat - 90, lon - 180, lat+TILE_STEP - 90, lon+TILE_STEP - 180);
                            cv = new OSMToSVGConverter(it->filePath().toStdString(), coords.toStdString(), *filter, svgFile.toStdString(), STYLE_SVG, b, tsize, tsize);
                            cv->draw(PATTERNS_FILE, 1);
                        }
                        png.setShift(shiftx, shifty);
                        png.convert(svgFile.toStdString(), pngDir.toStdString(), pngFile.toStdString());
                    }
                }
            }
            emit tileGenerated();
            delete cv;
        }
        delete it;
    }


}

DynamicTilesGenerator::DynamicTilesGenerator(const QString &dynamicMapDir, int size, const QVector<QCheckBox *> &checkedDist) :
    dynamicMapDir(dynamicMapDir),
    size(size),
    checkedDist(checkedDist)
{}
