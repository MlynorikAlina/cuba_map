#include "vectortextureloader.h"

#include <QDirIterator>
#include <QProcess>
#include <cmath>
#include <params.h>

VectorTextureLoader::VectorTextureLoader(const QString &textureFile, const QString &asterDir, Bbox box) : textureFile(textureFile),
    asterDir(asterDir), box(box)
{}

void VectorTextureLoader::run()
{
    QStringList args;

    args <<VEC_ASTER_PARSER_PY << asterDir <<textureFile<<QString::number(DEFAULT_TILE_SIZE);
    args<<QString::number(floor(box.minLon))<<QString::number(floor(box.minLat))<<QString::number(floor(box.maxLon))<<QString::number(floor(box.maxLat));
    QProcess* proc = new QProcess;
    proc->start(PYTHON, args);
    if (!proc->waitForStarted(-1) || !proc->waitForFinished(-1)) {
        return;
    }
}
