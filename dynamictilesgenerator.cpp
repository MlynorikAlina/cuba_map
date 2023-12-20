#include <math.h>
#include <tiles/png_converter/SVGToPNGConverter.h>
#include "dynamictilesgenerator.h"
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
        OverpassFilter *filter = SettingsScreen::getFilter(e.toInt());

        UpdateMode mode = updateMapParams(dynamicMapDir+e+"_p.txt", filter, "0","0");

        double frac = min(TILE_STEP, dynamicTilesSize[e].toDouble());
        int tsize = round(size*TILE_STEP/frac);
        QString tempAster = PARSED_ASTER + e + "/";
        QDir dir(tempAster);
        if (!dir.exists())
            dir.mkpath(dir.absolutePath());

        AsterParserDynamic *p = new AsterParserDynamic(ASTER_DIR, tempAster, QString::number(tsize), QString::number(TILE_STEP), QString::number(TILE_STEP_PREC));
        p->exec();

        QString outDir = dynamicMapDir + e + "/";
        dir.setPath(outDir);
        for(double i = b.minLat; i<b.maxLat; i+=TILE_STEP)
            for(double j = b.minLon; j<b.maxLon;j+=TILE_STEP) {
                QString fp;
                QTextStream s(&fp);
                s.setRealNumberNotation(QTextStream::FixedNotation);
                s.setRealNumberPrecision(1);
                s << OSM_DIR << i + 90. << "/" << j + 180. << ".osm";

                if (!dir.exists())
                    dir.mkpath(dir.absolutePath());

                dir.setPath(fp);
                dir.setPath(QDir::cleanPath(dir.filePath("../..")));

                QString coords = fp;
                coords.remove(".osm");
                coords.remove(dir.path());
                coords.remove(0, 1);
                coords.replace("/", "_");
                QStringList sl = coords.split("_");
                coords = tempAster + coords + ".json";

                lat = sl[0].toDouble();
                lon = sl[1].toDouble();
                prec = max(dynamicTilesPrec[e],TILE_STEP_PREC);

                OSMToSVGConverter* cv = NULL;

                SVGToPNGConverter png(size, size);
                png.setViewport(tsize);
                for (int shifty = 0; shifty < tsize; shifty += size){
                    QString pngDir = outDir;
                    QTextStream ss(&pngDir);
                    ss.setRealNumberPrecision(prec);
                    ss.setRealNumberNotation(QTextStream::FixedNotation);
                    ss << lat + TILE_STEP - 1. * shifty / tsize - frac << '/';
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
                                cv = new OSMToSVGConverter(fp.toStdString(), coords.toStdString(), *filter, svgFile.toStdString(), style.toStdString(), b, tsize, tsize);
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
    }

}

UpdateMode DynamicTilesGenerator::updateMapParams(QString fileOut,
                                                  OverpassFilter *filter, QString lat, QString lon) {
    QFile file(fileOut);
    UpdateMode needUpd = UpdateMode::NO_UPDATE;
    bool filterSame = false;
    if (!file.open(QIODevice::ReadOnly) || file.size() == 0) {
        file.close();
        file.open(QIODevice::WriteOnly);
        QTextStream s(&file);
        s << style << Qt::endl;
        s << lat << Qt::endl;
        s << lon;
        for (auto e : filter->getIncludedFeatures())
            s << Qt::endl << QString::fromStdString(mf::to_string(e));
        needUpd = UpdateMode::UPDATE_ALL;
    } else {
        QTextStream in(&file);
        QString prevStyle = in.readLine();
        QString tlat = in.readLine();
        QString tlon = in.readLine();
        set<mf::MapFeature> includes;
        while (!in.atEnd()) {
            includes.insert(mf::from_string(in.readLine().toStdString()));
        }
        if (includes == filter->getIncludedFeatures()) {
            filterSame = true;
        }
        bool diffrent_center = tlat.compare(lat) != 0 ||
                               tlon.compare(lon) != 0;
        if (diffrent_center || !filterSame || prevStyle!=style) {
            file.close();
            file.open(QIODevice::WriteOnly);
            QTextStream s(&file);
            s << style << Qt::endl;
            if (diffrent_center) {
                s << lat << Qt::endl;
                s << lon;
                needUpd = UpdateMode::UPDATE_ALL;
            } else {
                s << tlat << Qt::endl;
                s << tlon;
                needUpd = UpdateMode::UPDATE_FILTER;
            }
            for (auto e : filter->getIncludedFeatures())
                s << Qt::endl << QString::fromStdString(mf::to_string(e));
        }
    }
    file.close();

    return needUpd;
}

void DynamicTilesGenerator::setBorder(const Bbox &newB)
{
    b = newB;
}


DynamicTilesGenerator::DynamicTilesGenerator(const QString &dynamicMapDir, int size, QString style, const QVector<QString> &checkedDist) :
    dynamicMapDir(dynamicMapDir),
    size(size),
    style(style),
    checkedDist(checkedDist)
{}
