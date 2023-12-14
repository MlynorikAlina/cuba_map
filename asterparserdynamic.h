#ifndef ASTERPARSERDYNAMIC_H
#define ASTERPARSERDYNAMIC_H

#include <QThread>

class AsterParserDynamic : public QThread
{
    Q_OBJECT
public:
    AsterParserDynamic(QString asterDir, QString outDir, QString size, QString tileStep, QString tileStepPrec);
    void exec();
protected:
    void run();

private:
    QStringList params;
};

#endif // ASTERPARSERDYNAMIC_H
