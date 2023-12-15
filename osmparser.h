#ifndef OSMPARSER_H
#define OSMPARSER_H

#include <QThread>



class OSMParser : public QThread
{
    Q_OBJECT
public:
    OSMParser(const QString &osmDir, const QString & pbfDirName);
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

#endif // OSMPARSER_H
