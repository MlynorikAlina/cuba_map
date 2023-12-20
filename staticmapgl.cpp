#include "staticmapgl.h"
#include "params.h"
#include "tiles/GeoInfo.h"

#include <QFile>

StaticMapGL::StaticMapGL():
    tex_c({{0,0},{0,1},{1,1},{1,0}})
{
}

void StaticMapGL::setParams(const QString &par)
{
    dist = par;
    loadParams();
    makeCurrent();
    loadTexture();
    resizeGL(width(), height());
    update();
}

void StaticMapGL::loadParams()
{
    QFile f(STATIC_MAP_PARAMS_FILE);
    if(f.open(QFile::ReadOnly)){
        QTextStream ss(&f);;
        QStringList sl = ss.readLine().split(" ");
        c_lat = sl[0].toFloat();
        c_lon = sl[1].toFloat();
        setBounds();
    }

}

void StaticMapGL::paintGL()
{
    glLoadIdentity();
    glTranslatef(0.0, 0.0, 4.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);

    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    if(texture.defined()){
        texture.texture->bind();
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glVertexPointer(2, GL_DOUBLE, 0, texture.vertex_c);
        glTexCoordPointer(2, GL_INT, 0, tex_c.constData());
        glDrawArrays(GL_POLYGON, 0, 8);
    }


    glDisableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
    glDisable(GL_TEXTURE_2D);


    drawGrid();

    glColor3f(1,1,1);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void StaticMapGL::loadTexture()
{
    texture.setTexture(STATIC_MAP_DIR + dist + ".png", b.minLat,b.minLon,b.maxLat,b.maxLon);
}

void StaticMapGL::setBounds()
{
    double dist = this->dist.toDouble();
    b.minLon = c_lon - GeoInfo::km2long(dist,c_lat);
    b.maxLon = c_lon + GeoInfo::km2long(dist,c_lat);
    b.minLat = c_lat - GeoInfo::km2lat(dist);
    b.maxLat = c_lat + GeoInfo::km2lat(dist);

    generateGrid();
}
