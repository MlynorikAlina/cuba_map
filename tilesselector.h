#ifndef TILESSELECTOR_H
#define TILESSELECTOR_H

#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLWidget>

#include <FTGL/FTGLPixmapFont.h>

enum SelectionMode{NO_SELECT, ON_HOWER, ON_SELECT, SELECTED};

struct Selection
{
    int coords[8];
    SelectionMode mode;
};

class TilesSelector : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    TilesSelector();
    Selection getBox() const;

private:
    QVector<QPoint> tex_c;
    QVector<QPoint> vec_c;
    QVector<QPoint> grid_v_c;
    QVector<QPoint> grid_h_c;
    int latMin = 0;
    int latMax = 30;
    int lonMin = -90;
    int lonMax = -60;
    double textOff;
    uint8_t text_color[3];
    QOpenGLTexture* texture;
    FTPixmapFont font;
    Selection box;
    // QOpenGLWidget interface
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void renderText(double x, double y, const QString &str, uint8_t *color);

    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // TILESSELECTOR_H
