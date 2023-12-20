#include "textureloader.h"

#include <QDirIterator>
#include <QProcess>
#include <cmath>
#include <params.h>

TextureLoader::TextureLoader(const QString &textureFile, const QString &asterDir, Bbox box) : textureFile(textureFile),
    asterDir(asterDir), box(box), parser(VEC_ASTER_PARSER_PY)
{}

void TextureLoader::load()
{
    this->start();
}

void TextureLoader::run()
{
    QStringList args;

    args <<parser << asterDir <<textureFile<<QString::number(DEFAULT_TILE_SIZE);
    args<<QString::number(box.minLon,'f', 8)<<QString::number(box.minLat,'f', 8)<<QString::number(box.maxLon,'f', 8)<<QString::number(box.maxLat,'f', 8);
    QProcess* proc = new QProcess;
    proc->start(PYTHON, args);

    if (!proc->waitForStarted(-1) || !proc->waitForFinished(-1)) {
        return;
    }
    qDebug()<<proc->readAllStandardError();
    qDebug()<<proc->readAllStandardOutput();
}

TextureLoader::TextureLoader(const QString &textureFile, const QString &asterDir, const QString &parser, const Bbox &box) : textureFile(textureFile),
    asterDir(asterDir),
    parser(parser),
    box(box)
{}
