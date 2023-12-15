#include "params.h"
#include "dynamicmapgl.h"
#include "tiles/GeoInfo.h"

#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QTextStream>
#include <math.h>
#include <QKeyEvent>

DynamicMapGL::DynamicMapGL(QString params):
    tex_c({{0,0},{0,1},{1,1},{1,0}})
{
    paramsFile = params;
}

void DynamicMapGL::paintGL()
{
    glLoadIdentity();
    glTranslatef(0.0, 0.0, 0.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);

    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    for(auto e: texture){
        e->texture->bind();
        glVertexPointer(2, GL_DOUBLE, 0, e->vertex_c);
        glTexCoordPointer(2, GL_INT, 0, tex_c.constData());
        glDrawArrays(GL_POLYGON, 0, 8);
    }

    glDisableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
    glDisable(GL_TEXTURE_2D);


    drawGrid();

    glColor3f(1,1,1);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void DynamicMapGL::keyPressEvent(QKeyEvent *event)
{
    double dist = this->dist.toDouble();
    double dlon = GeoInfo::km2long(dist,c_lat);
    double dlat = GeoInfo::km2lat(dist);

    switch(event->key()){
    case Qt::Key_Left:
        move(0,-dlon);
        break;
    case Qt::Key_Right:
        move(0,dlon);
        break;
    case Qt::Key_Up:
        move(dlat,0);
        break;
    case Qt::Key_Down:
        move(-dlat,0);
        break;
    }
}

void DynamicMapGL::setParams(const QString &par)
{
    dist = par;
    loadParams();
    makeCurrent();
    resizeGL(width(), height());
    update();
}

void DynamicMapGL::loadParams()
{
    QFile f(paramsFile);
    if(f.open(QFile::ReadOnly)){
        QTextStream ss(&f);
        QStringList sl = ss.readLine().split(" ");
        tilesDir = ss.readLine() + dist + "/";
        c_lat = sl[0].toFloat();
        c_lon = sl[1].toFloat();
        setBounds();
        loadTiles();
    }

}

void DynamicMapGL::loadTiles()
{
    for(int i = 0; i<texture.size(); i++)
        delete texture[i];
    texture.clear();
    double frac = min(TILE_STEP, dynamicTilesSize[dist].toDouble());
    int prec = max(dynamicTilesPrec[dist],TILE_STEP_PREC);;

    for(double lat = floor((b.maxLat) / frac)*frac; lat>=floor((b.minLat) / frac)*frac; lat-=frac){
        QString ltext = tilesDir;
        QTextStream ss(&ltext);
        ss.setRealNumberNotation(QTextStream::FixedNotation);
        ss.setRealNumberPrecision(prec);
        ss<<lat+90;
        for(double lon = floor((b.minLon) / frac)*frac; lon<b.maxLon; lon+=frac){
            QString text = ltext + "/";
            ss.setString(&text);
            ss.setRealNumberNotation(QTextStream::FixedNotation);
            ss.setRealNumberPrecision(prec);
            ss<<lon+180<<".png";
            if(QFile::exists(text))
                texture.push_back(new TileInfo(text,lat,lon,lat+frac,lon+frac));
        }
    }

}

void DynamicMapGL::setBounds()
{
    double dist = this->dist.toDouble();
    b.minLon = c_lon - GeoInfo::km2long(dist,c_lat);
    b.maxLon = c_lon + GeoInfo::km2long(dist,c_lat);
    b.minLat = c_lat - GeoInfo::km2lat(dist);
    b.maxLat = c_lat + GeoInfo::km2lat(dist);

    generateGrid();
}

void DynamicMapGL::move(double dlat, double dlon)
{
    c_lat+=dlat;
    c_lon+=dlon;
    setBounds();
    loadTiles();
    makeCurrent();
    resizeGL(width(), height());
    update();
}


TileInfo::TileInfo(QString tName, double minLat, double minLon, double maxLat, double maxLon)
{
    texture = new QOpenGLTexture(QImage(tName).mirrored());
    vertex_c[0] = vertex_c[2] = minLon;
    vertex_c[1] = vertex_c[7] = minLat;
    vertex_c[4] = vertex_c[6] = maxLon;
    vertex_c[5] = vertex_c[3] = maxLat;
}

TileInfo::~TileInfo()
{
    delete texture;
}
