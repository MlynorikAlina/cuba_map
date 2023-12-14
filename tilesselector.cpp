#include "tilesselector.h"
#include "params.h"
#include "qdebug.h"
#include <QMouseEvent>
#include <math.h>
#include <string>

TilesSelector::TilesSelector(): font(FONT_LOC)
{
    font.FaceSize(TEXT_SIZE);
    for(int i = lonMin; i<lonMax; i+=PLANET_TILES_STEP){
        grid_v_c.append(QPoint(i, latMin));
        grid_v_c.append(QPoint(i, latMax));
    }
    for(int i = latMin; i<latMax; i+=PLANET_TILES_STEP){
        grid_h_c.append(QPoint(lonMin, i));
        grid_h_c.append(QPoint(lonMax, i));
    }
    vec_c = {{lonMin,latMin},{lonMax,latMin},{lonMax,latMax}, {lonMin,latMax}};
    tex_c = {{0,0},{1,0},{1,1},{0,1}};

    text_color[0] = text_color[1] = text_color[2] = 0x11;

    box.mode = NO_SELECT;
    this->setMouseTracking(true);
}

Selection TilesSelector::getBox() const
{
    return box;
}

void TilesSelector::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0,0,0.269,0);
#ifndef WIN32
    glBlendColor(0.0f, 0.0f, 0.0f, 0.5f);
#endif
    glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
    if(glIsEnabled(GL_MULTISAMPLE_ARB)){
        glDisable(GL_MULTISAMPLE_ARB);
    }
    texture = new QOpenGLTexture(QImage(WORLD_MAP_PNG).mirrored());
}

void TilesSelector::resizeGL(int w, int h)
{
    glViewport(0,0,w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(lonMin, lonMax, latMin, latMax, -1.0, 1.0);
    textOff = TEXT_SIZE*(double)(latMax-latMin)/height();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void TilesSelector::paintGL()
{
    glLoadIdentity();
    glTranslatef(0.0, 0.0, 0.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);

    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    texture->bind();
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_INT, 0, vec_c.constData());
    glTexCoordPointer(2, GL_INT, 0, tex_c.constData());
    glDrawArrays(GL_POLYGON, 0, vec_c.size());
    glDisableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
    glDisable(GL_TEXTURE_2D);


    glColor3f(0,0,0);
    glVertexPointer(2, GL_INT, 0, grid_v_c.constData());
    glDrawArrays(GL_LINES, 0, grid_v_c.size());
    for(int i=2; i<grid_v_c.size(); i+=2){
        renderText(grid_v_c[i].x() + 1, grid_v_c[i+1].y()-textOff, QString::number(grid_v_c[i].x()), text_color);
    }

    glVertexPointer(2, GL_INT, 0, grid_h_c.constData());
    glDrawArrays(GL_LINES, 0, grid_h_c.size());

    for(int i=2;i<grid_h_c.size();i+=2){
        renderText(grid_h_c[i].x(), grid_h_c[i].y() + 1, QString::number(grid_h_c[i].y()), text_color);
    }

    if(box.mode!=NO_SELECT){
        glEnable(GL_BLEND);
        glColor4ub(255,94,0,90);
        glVertexPointer(2, GL_INT, 0, box.coords);
        glDrawArrays(GL_POLYGON, 0, 4);
        glDisable(GL_BLEND);
    }


    glColor3f(1,1,1);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void TilesSelector::renderText(double x, double y, const QString &str, uint8_t *color)
{
    double _x = ((x - lonMin) * width()) / (lonMax-lonMin);
    double _y = ((y - latMin) * height()) / (latMax-latMin);
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

void TilesSelector::mouseMoveEvent(QMouseEvent *event)
{
    auto p = event->pos();
    int x = lonMin + floor((lonMax-lonMin)*p.x()/(width()*PLANET_TILES_STEP)*PLANET_TILES_STEP);
    int y = latMin + floor((latMax-latMin)*(height() - p.y())/(height()*PLANET_TILES_STEP)*PLANET_TILES_STEP);
    if(box.mode != ON_SELECT){
        if(box.mode != SELECTED){
            box.mode = ON_HOWER;
            box.coords[0] = box.coords[6] = x;
            box.coords[1] = box.coords[3] = y;
            box.coords[2] = box.coords[4] = x + PLANET_TILES_STEP;
            box.coords[5] = box.coords[7] = y + PLANET_TILES_STEP;
        }
    }else{
        if(x<box.coords[0]){
            box.coords[2] = box.coords[4] = x;
        }
        else
            box.coords[2] = box.coords[4] = x + PLANET_TILES_STEP;
        if(y<box.coords[5]){
            box.coords[1] = box.coords[3] = y;
        }
        else
            box.coords[1] = box.coords[3] = y + PLANET_TILES_STEP;

    }
    update();
}

void TilesSelector::leaveEvent(QEvent *event)
{
    if(box.mode!=SELECTED){
        box.mode = NO_SELECT;
        update();
    }
}

void TilesSelector::mousePressEvent(QMouseEvent *event)
{
    box.mode = ON_HOWER;
    mouseMoveEvent(event);
    box.mode = ON_SELECT;
}

void TilesSelector::mouseReleaseEvent(QMouseEvent *event)
{
    box.mode = SELECTED;
}

void TilesSelector::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Escape){
        box.mode = NO_SELECT;
        update();
    }

}




