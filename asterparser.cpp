#include "asterparser.h"
#include "params.h"
#include "qdebug.h"

#include <qprocess.h>

QHash<QString, QString> AsterParser::contoursFrac = {{"100","50"},{"70","40"},{"50","30"},{"35","25"},{"25","20"},{"20","15"},{"5","10"}};

AsterParser::AsterParser(QString asterDir, QString outDir, QString contoursFile, QString textureFile, QString size, QString lat, QString lon, QString dist)
{
    params << STATIC_ASTER_PARSER_PY << asterDir << outDir << contoursFile << textureFile
           << size << lat << lon << dist << contoursFrac[dist];
}

void AsterParser::run()
{
    QProcess proc;
    proc.start(PYTHON, params);
    if (!proc.waitForStarted(-1) || !proc.waitForFinished(-1)) {
        return;
    }
    if (proc.exitCode() != 0)
        qDebug() << proc.readAllStandardError();
}
