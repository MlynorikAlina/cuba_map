#include "osmloader.h"
#include "params.h"
#include "tiles/GeoInfo.h"

#include <QTextStream>
#include <QProcess>
#include <cmath>



OSMLoader::OSMLoader(double clat, double clon, double max_dist, OSMParserMode mode): mode(mode)
{
    double dlat = GeoInfo::km2long(max_dist,clat);
    double dlon = GeoInfo::km2lat(max_dist);
    double add_dlat = min(0.5, dlat/2);
    double add_dlon = min(0.5, dlon/2);
    if(mode!=_VECTOR)
        b.setBox(clat - dlat - add_dlat, clon - dlon - add_dlon, clat + dlat + add_dlat, clon + dlon + add_dlon);
    else
        b.setBox(clat - dlat, clon - dlon, clat + dlat, clon + dlon);
    setDefaultOutFile();
}

OSMLoader::OSMLoader(double minLat, double minLon, double maxLat, double maxLon, OSMParserMode mode): mode(mode)
{
    b.setBox(minLat, minLon, maxLat ,maxLon);
    setDefaultOutFile();
}

OSMLoader::OSMLoader(Bbox border, OSMParserMode mode): b(border), mode(mode)
{
    setDefaultOutFile();
}

Bbox OSMLoader::getBox() const
{
    return b;
}

void OSMLoader::load()
{
    start();
}

void OSMLoader::run()
{        
    QString box = getBoxStr();
    uint count = 0;
    for(int i = floor((b.minLat)/PLANET_TILES_STEP)*PLANET_TILES_STEP; i<=floor((b.maxLat)/PLANET_TILES_STEP)*PLANET_TILES_STEP;i+=PLANET_TILES_STEP)
        for(int j = floor((b.minLon)/PLANET_TILES_STEP)*PLANET_TILES_STEP; j<=floor((b.maxLon)/PLANET_TILES_STEP)*PLANET_TILES_STEP;j+=PLANET_TILES_STEP){
            parsePBF(box,PBF_DIR + QString::number(i + 90) + "_" + QString::number(j + 180) + ".osm.pbf", "tf" + QString::number(count) + ".osm");
            count++;
        }

    QStringList args;

    for(uint i=0;i<count;i++){
        args<< "tf" + QString::number(i) + ".osm";
    }
    args<<QString("-o=") + tempOsm;
    startProc(args);
}

int OSMLoader::getExitCode() const
{
    return exitCode;
}

void OSMLoader::setDefaultOutFile()
{
    if(mode==_STATIC){
        tempOsm = TMP_STAT_OSM;
    }else{
        tempOsm = TMP_VEC_OSM;
    }
}

void OSMLoader::parsePBF(QString inPBFFile, QString outFile)
{
    if(outFile.size()==0)
        outFile = tempOsm;
    parsePBF(getBoxStr(), inPBFFile, outFile);
}

void OSMLoader::parsePBF(QString b, QString inPBFFile, QString outFile)
{
    __TIME__
    QStringList args;
    args<<inPBFFile<<b<<"--drop-author" << "--drop-version";

    if(mode!=_STATIC)
        args<<"--drop-broken-refs";
    if(mode==_VECTOR)
        args<<"--complete-ways";
    args<<QString("-o=") + outFile;

    startProc(args);

    __TIME__
}

void OSMLoader::startProc(QStringList args)
{
    QProcess proc;
    proc.start("osmconvert", args);
    if (!proc.waitForStarted(-1) || !proc.waitForFinished(-1)) {
        return ;
    }

    exitCode = proc.exitCode();
    if (exitCode != 0)
        qDebug() << proc.readAllStandardError();
}

QString OSMLoader::getBoxStr()
{
    QString box = "-b=";
    QTextStream ss(&box);
    ss.setRealNumberNotation(QTextStream::FixedNotation);

    ss<<b.minLon<<","<<b.minLat<<","<<b.maxLon<<","<<b.maxLat;
    return box;
}

void OSMLoader::setTempOsm(const QString &newTempOsm)
{
    tempOsm = newTempOsm;
}

void OSMLoader::exec()
{
    run();
}

