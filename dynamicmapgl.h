#ifndef DYNAMICMAPGL_H
#define DYNAMICMAPGL_H

#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <mapgl.h>

#include <tiles/OverpassFilter.h>

#include <FTGL/FTGLPixmapFont.h>


struct TileInfo{
    QOpenGLTexture* texture;
    double vertex_c[8];
    TileInfo();
    TileInfo(QString tName, double minLat, double minLon, double maxLat, double maxLon);
    void setTexture(QString tName, double minLat, double minLon, double maxLat, double maxLon);
    bool defined();
    ~TileInfo();
};


class DynamicMapGL : public MapGL
{

public:
    DynamicMapGL(QString params, QWidget* parent = nullptr);
    ~DynamicMapGL();
    // QOpenGLWidget interface
protected:
    void paintGL();
    void keyPressEvent(QKeyEvent *event);

private:
    QVector<TileInfo*> texture;

    QVector<QPoint> tex_c;

    QString paramsFile;
    QString dist;
    QString tilesDir;
    void loadParams();
    void loadTiles();
    void setBounds();
    void move(double dlat, double dlon);
    // MapGL interface
public:
    void setParams(const QString &par);

};

#endif // DYNAMICMAPGL_H
