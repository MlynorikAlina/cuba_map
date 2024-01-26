#include "osmtilesgenerator.h"
#include "qdebug.h"

#include <QDir>
#include <QProcess>
#include <QTextStream>
#include <math.h>
#include <osmloader.h>
#include "params.h"


OSMTilesGenerator::OSMTilesGenerator(const QString &osmDir, const QString &pbfDirName) : osmDir(osmDir),
    pbfDirName(pbfDirName)
{
    QDir d(TEMP_FILE_DIR);
    d.removeRecursively();
    d.mkpath(d.absolutePath());
}

void OSMTilesGenerator::putBorderPoint(int p)
{
    if(list.size()<4)
        list.push_back(p);
}

void OSMTilesGenerator::run()
{
    QDir dir(TEMP_FILE_DIR);
    if (!dir.exists())
        dir.mkpath(dir.absolutePath());
    for(int i = PLANET_TILES_STEP*floor(list[2]/PLANET_TILES_STEP); i<list[3];i+=PLANET_TILES_STEP)
        for(int j = PLANET_TILES_STEP*floor(list[0]/PLANET_TILES_STEP); j<list[1];j+=PLANET_TILES_STEP){
            QString n;
            QTextStream s(&n);
            s << WORLD_PBF_DIR << i + 90 << "_" << j + 180 << ".osm.pbf";
            parseOsm(std::max(list[2],i), std::max(list[0],j), std::min(list[3],i+PLANET_TILES_STEP), std::min(list[1],j+PLANET_TILES_STEP), n);
        }
    dir.rmdir(dir.absolutePath());
}

void OSMTilesGenerator::parseOsm(double lat, double lon, QString pbf)
{
    QString fn;
    QTextStream s(&fn);
    s.setRealNumberNotation(QTextStream::FixedNotation);
    s.setRealNumberPrecision(1);
    s << osmDir << lat + 90. << "/" << lon + 180. << ".osm";
    if (!QFile::exists(fn)) {
        QDir dir(fn);
        dir.setPath(QDir::cleanPath(dir.filePath("../")));
        if (!dir.exists())
            dir.mkpath(dir.absolutePath());
        OSMLoader ol(lat - 0.5*TILE_STEP, lon - 0.5*TILE_STEP, lat + 1.5*TILE_STEP, lon + 1.5*TILE_STEP);
        ol.setTempOsm(fn);
        ol.parsePBF(pbf, fn);
    }
    emit fileParsed();
}

void OSMTilesGenerator::parseOsm(double minlat, double minlon, double maxlat, double maxlon, QString pbf)
{
    double clat = floor((minlat+maxlat)/(2*TILE_STEP))*TILE_STEP;
    double clon = floor((minlon+maxlon)/(2*TILE_STEP))*TILE_STEP;
    if(clat-minlat<=TILE_STEP || clon-minlon<=TILE_STEP){
        for(double lat = minlat; lat<maxlat; lat+=TILE_STEP)
            for(double lon = minlon; lon<maxlon;lon+=TILE_STEP)
                parseOsm(lat, lon, pbf);
    }else{
        bool needsParse = false;
        for(double lat = minlat; lat<maxlat; lat+=TILE_STEP)
            for(double lon = minlon; lon<maxlon;lon+=TILE_STEP){
                QString p;
                QTextStream s(&p);
                s.setRealNumberPrecision(1);
                s.setRealNumberNotation(QTextStream::FixedNotation);
                s << osmDir << lat + 90 << "/" << lon + 180 << ".osm";
                if (!QFile::exists(p)){
                    needsParse = true;
                    break;
                }
            }
        if(needsParse){
            QList<double> lat = {minlat, clat, maxlat};
            QList<double> lon = {minlon, clon, maxlon};
            for(int i = 0; i<2; i++)
                for(int j = 0; j<2; j++){
                    QString tfn = TEMP_FILE_DIR + QString::number(tf_count++) + ".osm.pbf";
                    OSMLoader ol(lat[i], lon[j], lat[i+1], lon[j+1], _VECTOR);
                    ol.parsePBF(pbf, tfn);
                    if (ol.getExitCode() == 0){
                        parseOsm(lat[i], lon[j], lat[i+1], lon[j+1], tfn);
                    }
                    QFile(tfn).remove();
                }
        }else{
            for(double lat = minlat; lat<maxlat; lat+=TILE_STEP)
                for(double lon = minlon; lon<maxlon;lon+=TILE_STEP){
                    emit fileParsed();
                }
        }
    }
}

void OSMTilesGenerator::setList(QVector<int> &newList)
{
    list.clear();
    list.append(newList);
}

