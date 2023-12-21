#ifndef ASTERPARSER_H
#define ASTERPARSER_H

#include <QThread>

#include <tiles/OverpassFilter.h>

class AsterParser : public QThread
{
public:
    explicit AsterParser(QObject *parent = nullptr);
    void setDynamicArgs(QString asterDir, QString outDir, int size, double tileStep, int tileStepPrec);
    void setStaticArgs(QString textureFile, QString asterDir, int size, Bbox border);
    void setVectorArgs(QString textureFile, QString asterDir, Bbox box);
    void exec();
public slots:
    void load();
protected:
    void run();

private:
    QStringList args;
};

#endif // ASTERPARSER_H
