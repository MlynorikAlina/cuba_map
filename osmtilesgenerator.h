#ifndef OSMTILESGENERATOR_H
#define OSMTILESGENERATOR_H

#include <QThread>



class OSMTilesGenerator : public QThread
{
    Q_OBJECT
public:
    OSMTilesGenerator(const QString &osmDir, const QString & pbfDirName);
    void putBorderPoint(int p);

    void setList(QVector<int> &newList);

signals:
    void fileParsed();
protected:
    void run();

private:
    void parseOsm(double lat, double lon, QString pbf);
    void parseOsm(double minlat, double minlon, double maxlat, double maxlon, QString pbf);
    int tf_count;
    QString osmDir;
    QVector<int> list;
    QString pbfDirName;

};

#endif // OSMTILESGENERATOR_H
