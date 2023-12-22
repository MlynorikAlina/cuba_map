#ifndef OSMLOADER_H
#define OSMLOADER_H

#include <QThread>

#include <tiles/OverpassFilter.h>

enum OSMParserMode{
    _STATIC, _VECTOR, _STATIC_DROP_REFS
};

class OSMLoader : public QThread
{
public:
    explicit OSMLoader(double clat, double clon, double max_dist, OSMParserMode mode = _STATIC_DROP_REFS);
    OSMLoader(Bbox border, OSMParserMode mode = _STATIC_DROP_REFS);
    OSMLoader(double minLat, double minLon, double maxLat, double maxLon, OSMParserMode mode = _STATIC_DROP_REFS);
    Bbox getBox() const;
    void setTempOsm(const QString &newTempOsm);
    void exec();
    void parsePBF(QString inPBFFile, QString outFile = "");
    int getExitCode() const;

public slots:
    void load();
    // QThread interface
protected:
    void run();
private:
    Bbox b;
    QString tempOsm;
    OSMParserMode mode;
    int exitCode;
    void setDefaultOutFile();
    void parsePBF(QString b, QString inPBFFile, QString outFile);
    void startProc(QStringList args);
    QString getBoxStr();
};

#endif // OSMLOADER_H
