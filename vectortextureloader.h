#ifndef VECTORTEXTURELOADER_H
#define VECTORTEXTURELOADER_H

#include <QThread>
#include <QObject>

#include <tiles/OverpassFilter.h>

class VectorTextureLoader : public QThread
{
    Q_OBJECT
private:
    QString textureFile;
    QString asterDir;
    Bbox box;
public:
    VectorTextureLoader(const QString &textureFile, const QString &asterDir, Bbox box);
signals:
    void textureLoaded();

    // QThread interface
protected:
    void run();
};

#endif // VECTORTEXTURELOADER_H
