#include "mapgl.h"
#include "params.h"
#include <GL/glu.h>
#include <QMouseEvent>
#include <math.h>

MapGL::MapGL() : font(FONT_LOC)
{
    font.FaceSize(TEXT_SIZE);
    text_color[0] = 0x11;
    text_color[1] = 0x11;
    text_color[2] = 0x11;
}

void MapGL::renderText(double x, double y, const QString &str, uint8_t *color)
{
    double _x = ((x - b.minLon) * width()) / (b.maxLon - b.minLon);
    double _y = ((y - b.minLat) * height()) / (b.maxLat - b.minLat);

    if (color != NULL) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);

        glPixelTransferf(GL_RED_BIAS, (1 / 255.0) * color[0] - 1);
        glPixelTransferf(GL_GREEN_BIAS, (1 / 255.0) * color[1] - 1);
        glPixelTransferf(GL_BLUE_BIAS, (1 / 255.0) * color[2] - 1);
    }

    font.Render(str.toStdString().c_str(), -1, FTPoint(_x, _y));

    if (color != NULL) {
        glPopAttrib();
    }
}

void MapGL::generateGrid()
{
    int lat_st = 1;
    int lon_st = 1;
    lon_deg = 0;
    lat_deg = 0;
    lon_v_c.clear();
    lat_v_c.clear();
    while((b.maxLon-b.minLon)*lon_st < MIN_GRID_LINES_COUNT - 1){
        lon_st*=10;
        lon_deg++;
    }
    while((b.maxLat-b.minLat)*lat_st < MIN_GRID_LINES_COUNT - 1){
        lat_st*=10;
        lat_deg++;
    }
    lon_st = max(lon_st, lat_st);
    lat_st = lon_st;
    lon_deg = max(lon_deg, lat_deg);
    lat_deg = lon_deg;
    for(double lon = floor(b.minLon*lon_st)/lon_st; lon<= ceil(b.maxLon*lon_st)/lon_st; lon+=1./lon_st){
        lon_v_c.append({lon, b.minLat});
        lon_v_c.append({lon, b.maxLat});
        lon_v_t.append(lon);
    }
    for(double lat = floor(b.minLat*lat_st)/lat_st; lat<= ceil(b.maxLat*lat_st)/lat_st; lat+=1./lat_st){
        lat_v_c.append({b.minLon,lat});
        lat_v_c.append({b.maxLon,lat});
        lat_v_t.append(lat);
    }
    lat_offset = (b.maxLat-b.minLat)*TEXT_SIZE/height();
}

void MapGL::drawGrid()
{
    glLineWidth(1);
    glColor3ub(GRID_COLOR);

    glVertexPointer(2, GL_DOUBLE, 0, lat_v_c.constData());
    glDrawArrays(GL_LINES, 0, lat_v_c.size()/2);
    for(int i = 0; i< lat_v_c.size();i+=4)
        renderText(lat_v_c[i], lat_v_c[i+1], QString::number(lat_v_c[i+1],'f',lat_deg), text_color);

    glVertexPointer(2, GL_DOUBLE, 0, lon_v_c.constData());
    glDrawArrays(GL_LINES, 0, lon_v_c.size()/2);
    for(int i = 0; i< lon_v_c.size();i+=4)
        renderText(lon_v_c[i+2], lon_v_c[i+3] - lat_offset, QString::number(lon_v_c[i+2],'f',lon_deg), text_color);

    glColor3f(1,1,1);
}

QPoint MapGL::transformCoordinates(double lon, double lat)
{
    return QPoint(round((lon-b.minLon)*lon_tr_coeff),round((lat-b.minLat)*lat_tr_coeff));
}



void MapGL::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(161./255,247./255,1,0);
#ifndef WIN32
    glBlendColor(0.0f, 0.0f, 0.0f, 0.5f);
#endif
    glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
    if(glIsEnabled(GL_MULTISAMPLE_ARB)){
        glDisable(GL_MULTISAMPLE_ARB);
    }
}

void MapGL::resizeGL(int w, int h)
{
    glViewport(0,0,w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(b.minLon, b.maxLon, b.minLat, b.maxLat, -10.0, -1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
