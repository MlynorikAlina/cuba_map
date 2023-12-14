#ifndef ASTERPARSER_H
#define ASTERPARSER_H

#include <QString>
#include <QThread>



class AsterParser : public QThread
{
public:
    AsterParser(QString asterDir, QString outDir, QString contoursFile, QString textureFile, QString size, QString lat, QString lon, QString dist);

    // QThread interface
protected:
    void run();

private:
    QStringList params;
    static QHash<QString, QString> contoursFrac;
};

#endif // ASTERPARSER_H
