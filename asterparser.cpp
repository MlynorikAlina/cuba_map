#include "asterparser.h"
#include <QProcess>
#include "params.h"

AsterParser::AsterParser(QObject *parent)
    : QThread{parent}
{

}

void AsterParser::setDynamicArgs(QString asterDir, QString outDir, int size, double tileStep, int tileStepPrec)
{
    args.clear();
    args << DYNAMIC_ASTER_PARSER_PY << asterDir << outDir << QString::number(size) <<QString::number(tileStep,'f', tileStepPrec)<<QString::number(tileStepPrec);
}

void AsterParser::setStaticArgs(QString textureFile, QString asterDir, int size, Bbox border)
{
    args.clear();
    args <<STAT_ASTER_PARSER_PY << asterDir << textureFile <<QString::number(size);
    args<<QString::number(border.minLon,'f', 8)<<QString::number(border.minLat,'f', 8)<<QString::number(border.maxLon,'f', 8)<<QString::number(border.maxLat,'f', 8);
}

void AsterParser::setVectorArgs(QString textureFile, QString asterDir, Bbox box)
{
    args.clear();
    args <<VEC_ASTER_PARSER_PY << asterDir <<textureFile<<QString::number(DEFAULT_TILE_SIZE);
    args<<QString::number(box.minLon,'f', 8)<<QString::number(box.minLat,'f', 8)<<QString::number(box.maxLon,'f', 8)<<QString::number(box.maxLat,'f', 8);
}

void AsterParser::load()
{
    start();
}

void AsterParser::run()
{
    QProcess* proc = new QProcess;
    proc->start(PYTHON, args);

    if (!proc->waitForStarted(-1) || !proc->waitForFinished(-1)) {
        return;
    }
    QString err = proc->readAllStandardError();
    if(err.size()>0)
        qInfo()<<err;
    //qInfo()<<proc->readAllStandardOutput();
}

void AsterParser::exec()
{
    run();
}
