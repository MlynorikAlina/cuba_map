#include "osmparser.h"
#include "qdebug.h"

#include <QDir>
#include <QProcess>
#include <QTextStream>
#include <math.h>
#include "params.h"


OSMParser::OSMParser(const QString &osmDir, const QString &pbfDirName) : osmDir(osmDir),
    pbfDirName(pbfDirName)
{
    QDir d(TEMP_FILE_DIR);
    d.removeRecursively();
    d.mkpath(d.absolutePath());
}

void OSMParser::putBorderPoint(int p)
{
    if(list.size()<4)
        list.push_back(p);
}

void OSMParser::run()
{
    QDir dir(TEMP_FILE_DIR);
    if (!dir.exists())
        dir.mkpath(dir.absolutePath());
    for(int i = PLANET_TILES_STEP*floor(list[2]/PLANET_TILES_STEP); i<list[3];i+=PLANET_TILES_STEP)
        for(int j = PLANET_TILES_STEP*floor(list[0]/PLANET_TILES_STEP); j<list[1];j+=PLANET_TILES_STEP){
            QString n;
            QTextStream s(&n);
            s << "planet/" << i + 90 << "_" << j + 180 << ".osm.pbf";
            parseOsm(max(list[2],i), max(list[0],j), min(list[3],i+PLANET_TILES_STEP), min(list[1],j+PLANET_TILES_STEP), n);
            //__TIME__
    }
    dir.rmdir(dir.absolutePath());
}

void OSMParser::parseOsm(double lat, double lon, QString pbf)
{
    QStringList args;
    QString b, p;
    QTextStream s(&p);
    s.setRealNumberNotation(QTextStream::FixedNotation);
    s.setRealNumberPrecision(1);
    s << osmDir << lat + 90. << "/" << lon + 180. << ".osm";
    if (!QFile::exists(p)) {
        QDir dir(p);
        s.setString(&b);
        p.push_front("-o=");
        s <<"-b="<< lon - 0.5*TILE_STEP << "," << lat - 0.5*TILE_STEP << "," << lon + 1.5*TILE_STEP << "," << lat + 1.5*TILE_STEP;
        args << pbf <<  b << p << "--drop-author" << "--drop-version"<<"--drop-broken-refs";
        dir.setPath(QDir::cleanPath(dir.filePath("../")));
        if (!dir.exists())
            dir.mkpath(dir.absolutePath());
        QProcess proc;
        proc.start("osmconvert", args);
        if (!proc.waitForStarted(-1) || !proc.waitForFinished(-1)) {
            return;
        }
        if (proc.exitCode() != 0)
            qDebug() << proc.readAllStandardError();
    }
    emit fileParsed();
}

void OSMParser::parseOsm(double minlat, double minlon, double maxlat, double maxlon, QString pbf)
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
                    QString b, f;
                    QTextStream s(&b);
                    s.setRealNumberPrecision(1);
                    s.setRealNumberNotation(QTextStream::FixedNotation);
                    s <<"-b="<< lon[j] << "," << lat[i] << "," << lon[j+1] << "," << lat[i+1];
                    f = "-o=" + tfn;
                    QStringList args;
                    args << pbf <<  b << f <<"--complete-ways"<< "--drop-author" << "--drop-version" <<"--drop-broken-refs";
                    QProcess proc;
                    proc.start("osmconvert", args);
                    if (!proc.waitForStarted(-1) || !proc.waitForFinished(-1)) {
                        return;
                    }
                    if (proc.exitCode() != 0)
                        qDebug() << proc.readAllStandardError();
                    else{
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

void OSMParser::setList(QVector<int> &newList)
{
    list.clear();
    list.append(newList);
}

