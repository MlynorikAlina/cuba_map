#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <QThread>
#include <QObject>

#include <tiles/OverpassFilter.h>

class TextureLoader : public QThread
{
    Q_OBJECT
private:
    QString textureFile;
    QString asterDir;
    QString parser;
    Bbox box;
public:
    TextureLoader(const QString &textureFile, const QString &asterDir, Bbox box);
    TextureLoader(const QString &textureFile, const QString &asterDir, const QString &parser, const Bbox &box);

signals:
    void textureLoaded();
public slots:
    void load();
    // QThread interface
protected:
    void run();
};

#endif // TEXTURELOADER_H
