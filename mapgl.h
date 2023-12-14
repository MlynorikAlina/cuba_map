#ifndef MAPGL_H
#define MAPGL_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include <FTGL/FTGLPixmapFont.h>

#include <tiles/OverpassFilter.h>



class MapGL : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    MapGL(); 
    virtual void setParams(const QString &par) = 0;

protected:
    void initializeGL();
    void resizeGL(int w, int h);

    double lat_tr_coeff;
    double lon_tr_coeff;

    int lon_deg;
    int lat_deg;
    double lat_offset;
    double c_lat;
    double c_lon;
    uint8_t text_color[3];

    FTPixmapFont font;
    QVector<double> lat_v_c;
    QVector<double> lon_v_c;
    QVector<double> lat_v_t;
    QVector<double> lon_v_t;
    Bbox b;

    void renderText(double x, double y, const QString &str, uint8_t *color);
    void generateGrid();
    void drawGrid();
    QPoint transformCoordinates(double lon, double lat);
};

#endif // MAPGL_H
