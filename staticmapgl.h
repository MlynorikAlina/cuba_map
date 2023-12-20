#ifndef STATICMAPGL_H
#define STATICMAPGL_H

#include "dynamicmapgl.h"

#include <mapgl.h>



class StaticMapGL: public MapGL
{

public:
    StaticMapGL();
    void setParams(const QString &par);
    void loadParams();

protected:
    void paintGL();
    TileInfo texture;
    QString dist;
    QVector<QPoint> tex_c;
private:
    void loadTexture();
    void setBounds();
};

#endif // STATICMAPGL_H
