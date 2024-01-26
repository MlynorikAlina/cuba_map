#ifndef VECTORMAPGL_H
#define VECTORMAPGL_H

#include "mapgl.h"

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QProgressBar>
#include <QThread>

#include <tiles/OverpassFilter.h>

#include <FTGL/FTGLPixmapFont.h>

#include <tiles/svg_converter/OSMData.h>
#include <tiles/svg_converter/OSMParser.h>

struct WStyle{
    GLenum mode;
    uint8_t* color;
    uint8_t stroke;
    std::string id;

    WStyle(std::string id, GLenum mode, uint8_t* color, uint8_t str):mode(mode), color(color), stroke(str), id(id){}
};

struct IBbox{
    int minLat;
    int maxLat;
    int minLon;
    int maxLon;
public:
    void set(int minLat, int minLon, int maxLat, int maxLon);
};

class VectorMapDataLoader: public QThread
{
public:
    VectorMapDataLoader(QHash<QString, uint8_t *> *style_color, QHash<QString, uint8_t> *style_stroke, Bbox* b);

    const std::map<std::string, WayData>& getWayTags() const;
    const QVector<QPair<WStyle, QVector<double> > >& getLines() const;

private:
    void loadTexture(QString file);
    std::map<std::string,WayData> wayTags;
    QVector<QPair<WStyle, QVector<double>>> lines;
    QHash<QString, uint8_t*> *style_color;
    QHash<QString, uint8_t> *style_stroke;
    OSMParse parser;
    Bbox prev;
    Bbox* b;
protected:
    void run();
};

class VectorMapGL : public MapGL
{
public:
    VectorMapGL(QWidget* parent = nullptr);

    // QOpenGLWidget interface
    void setBounds();
    void loadParams();
    void setParams(const QString &par);
protected:
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
private:
    void updateStyle();
    QString dist;
    QString osmFileDir;
    QString style;

    QHash<QString, uint8_t*> style_color;
    QHash<QString, uint8_t> style_stroke;
    VectorMapDataLoader* vml;

    QProgressBar* progress;

};




#endif // VECTORMAPGL_H
