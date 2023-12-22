#ifndef VECTORMAPGL_H
#define VECTORMAPGL_H

#include "mapgl.h"

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include <tiles/OverpassFilter.h>

#include <FTGL/FTGLPixmapFont.h>

#include <tiles/svg_converter/OSMData.h>
#include <tiles/svg_converter/OSMParser.h>

struct WStyle{
    GLenum mode;
    uint8_t* color;
    uint8_t stroke;
    string id;

    WStyle(string id, GLenum mode, uint8_t* color, uint8_t str):mode(mode), color(color), stroke(str), id(id){}
};

struct IBbox{
    int minLat;
    int maxLat;
    int minLon;
    int maxLon;
public:
    void set(int minLat, int minLon, int maxLat, int maxLon);
};

class VectorMapGL : public MapGL
{
public:
    VectorMapGL();

    // QOpenGLWidget interface
    void setBounds();
    void loadParams();
    void loadData();
    void loadTexture(QString file);
    void setParams(const QString &par);
protected:
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
private:
    void updateStyle();
    
    OSMParse parser;
    QString dist;
    QString osmFileDir;
    QString style;
    map<string,WayData> wayTags;

    Bbox prev;

    QHash<QString, uint8_t*> style_color;
    QHash<QString, uint8_t> style_stroke;
    QVector<QPair<WStyle, QVector<double>>> lines;

};



#endif // VECTORMAPGL_H
