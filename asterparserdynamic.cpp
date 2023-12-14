#include "asterparserdynamic.h"
#include "params.h"
#include "qdebug.h"

#include <QProcess>

AsterParserDynamic::AsterParserDynamic(QString asterDir, QString outDir, QString size, QString tileStep, QString tileStepPrec)
{
    params << DYNAMIC_ASTER_PARSER_PY << asterDir << outDir << size <<tileStep<<tileStepPrec;
}

void AsterParserDynamic::exec()
{
    run();
}

void AsterParserDynamic::run()
{
    QProcess proc;
    proc.start(PYTHON, params);
    if (!proc.waitForStarted() || !proc.waitForFinished(-1)) {
        return;
    }
    if (proc.exitCode() != 0)
        qDebug() << proc.readAllStandardError();
}
