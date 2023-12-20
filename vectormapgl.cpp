#include "vectormapgl.h"
#include "settingsscreen.h"
#include "tiles/GeoInfo.h"

#include "params.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QTextStream>
#include <clickdetector.h>
#include <iomanip>
#include <math.h>
#include <qevent.h>
#include <sstream>


VectorMapGL::VectorMapGL():  parser(OverpassFilter())
{
    style = SettingsScreen::getVecStyle();
    updateStyle();
    prev.setBox(181, 361, -1, -1);
}


void VectorMapGL::paintGL()
{
    __TIME__
    glLineWidth(1);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, 4.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);

    for(auto e:lines){
        glColor3ub(e.first.color[0],e.first.color[1],e.first.color[2]);
        glLineWidth(e.first.stroke);
        glVertexPointer(2, GL_DOUBLE, 0, e.second.constData());
        glDrawArrays(e.first.mode, 0, e.second.size()/2);
    }

    drawGrid();

    glColor3f(1,1,1);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void VectorMapGL::mousePressEvent(QMouseEvent *event)
{
    auto p = event->pos();
    int x = event->x();
    int y = height() - event->y();
    double xd = x/lon_tr_coeff + b.minLon;
    double yd = y/lat_tr_coeff + b.minLat;

    for(auto e:lines){
        if(e.first.id.size()>0 && wayTags[e.first.id].getBorder().isIn(xd, yd)){
            bool selected = false;
            /*for (int i = 0; i < e.second.size(); i+=2) {
                if(select.isIn(e.second[i].x(), e.second[i].y())){
                    selected = true;
                    break;
                }
            }*/
            switch (e.first.mode) {
            case GL_POLYGON:
                selected = ClickDetector::inPolygon(e.second, xd, yd);
                break;
            case GL_LINE_STRIP:
                selected = ClickDetector::onLine(e.second, xd, yd);
                break;
            }
            if(selected){
                e.first.color[0]=0xff;e.first.color[0]=0x00;e.first.color[0]=0x00;
                for(auto tg: wayTags[e.first.id].getAttributes().getTags())
                    qDebug()<<QString::fromStdString(tg.first)<<" "<<QString::fromStdString(tg.second);
                qDebug()<<Qt::endl;
            }
        }
    }
    update();
}

void VectorMapGL::updateStyle()
{
    QString val;
    QFile file(style);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject json = doc.object();

    style_color.clear();
    style_stroke.clear();
    QHash<QString, QVariant> m = json.toVariantHash();
    QHashIterator<QString, QVariant> it = m;
    while(it.hasNext()){
        it.next();
        QStringList features = it.key().split(' ');
        QJsonObject prop = it.value().toJsonObject();
        uint8_t* color = NULL;
        uint8_t stroke = prop["stroke"].toInt(0);
        if(prop.contains("fill")){
            color = new uint8_t[3];
            QJsonArray arr = prop["fill"].toArray();
            for(int i=0;i<3;i++)
                color[i] = arr[i].toString().toInt(nullptr, 16);
        }
        for(auto f:features){
            if(color!=NULL)
                style_color.insert(f,color);
            if(stroke!=0)
                style_stroke.insert(f, stroke);
        }
    }
}



void VectorMapGL::setBounds()
{
    double dist = this->dist.toDouble();
    b.minLon = c_lon - GeoInfo::km2long(dist,c_lat);
    b.maxLon = c_lon + GeoInfo::km2long(dist,c_lat);
    b.minLat = c_lat - GeoInfo::km2lat(dist);
    b.maxLat = c_lat + GeoInfo::km2lat(dist);

    lon_tr_coeff = width()/(b.maxLon-b.minLon);
    lat_tr_coeff = height()/(b.maxLat-b.minLat);

    generateGrid();
}

void VectorMapGL::loadParams()
{
    QFile f(VECTOR_MAP_PARAMS_FILE);
    if(f.open(QFile::ReadOnly)){
        QTextStream ss(&f);
        QStringList sl = ss.readLine().split(" ");
        c_lat = sl[0].toDouble();
        c_lon = sl[1].toDouble();
        osmFileDir = ss.readLine();
        setBounds();
        loadData();
    }

}

void VectorMapGL::loadData()
{

    map<string,NodeAttributes> nodes;
    list<pair<string, WayData>>  ways;

    parser.setFilterBbox(b.minLon,b.minLat,b.maxLon,b.maxLat);
    parser.setOsmFile(TMP_VEC_OSM);
    parser.appendNodes(nodes);
    parser.appendWays(ways, nodes);
    loadTexture(TMP_VEC_TEXTURE);

    ways.sort(wc());

    prev.setBox(min(floor(b.minLat/TILE_STEP)*TILE_STEP+90, prev.minLat), min(floor(b.minLon/TILE_STEP)*TILE_STEP+180, prev.minLon), max(floor(b.maxLat/TILE_STEP)*TILE_STEP+90, prev.maxLat), max(floor(b.maxLon/TILE_STEP)*TILE_STEP+180, prev.maxLon));

    for (auto w: ways) {
        Bbox box;
        w.second.getBorder();
        QVector<double> points;
        uint8_t* color = new uint8_t[3];
        color[0] = 0xd3, color[1] = 0xd2, color[2] = 0xd2;
        uint8_t stroke = 1;
        GLenum mode = GL_POLYGON;
        bool display = false;
        for (auto tg: w.second.getAttributes().getTags()) {
            QString f = QString::fromStdString(tg.first);
            QString f1 = f + "-" + QString::fromStdString(tg.second);
            if(style_color.contains(f)){
                display = true;
                for(int i=0;i<3;i++)
                    color[i] = style_color[f][i];
            }
            if(style_color.contains(f1)){
                display = true;
                for(int i=0;i<3;i++)
                    color[i] = style_color[f1][i];
            }


            if(style_stroke.contains(f)){
                display = true;
                stroke = style_stroke[f];
                mode = GL_LINE_STRIP;
            }
            if(style_stroke.contains(f1)){
                display = true;
                stroke = style_stroke[f1];
                mode = GL_LINE_STRIP;
            }

        }
        if(display){
            for (auto n: w.second.getNodeRefs()) {
                points.push_back(n->getLon());
                points.push_back(n->getLat());
            }
            if(points.size()<3 || abs(points[0] - points[points.size()-2]) > 0 || abs(points[1] - points[points.size()-1]) > 0)
                mode = GL_LINE_STRIP;
            lines.push_back({WStyle(w.first, mode, color, stroke),points});
            wayTags.insert({w.first, w.second});
        }


    }
}

void VectorMapGL::loadTexture(QString file)
{
    //FIXME::
    QFile f(file);
    QString s;
    f.open(QIODevice::ReadOnly);
    uint8_t* color = new uint8_t[3];
    color[0] = 0xdc, color[1] = 0xdc, color[2] = 0xdc;
    while(!f.atEnd()){
        s = f.readLine();
        QJsonDocument doc = QJsonDocument::fromJson(s.toUtf8());
        QJsonObject arr = doc.object();
        QJsonArray v = arr["coordinates"].toArray();
        if(v.size()>0){
            QVector<double> points;
            for(auto e: v){
                points.push_back(e.toArray()[0].toDouble());
                points.push_back(e.toArray()[1].toDouble());
            }
            lines.push_back({WStyle("",GL_POLYGON, color, 1),points});
        }
    }
}

void VectorMapGL::setParams(const QString &par)
{
    dist = par;
    QString newStyle = SettingsScreen::getVecStyle();
    if(newStyle!=style){
        style = newStyle;
        updateStyle();
    }
    makeCurrent();
    loadParams();
    resizeGL(width(), height());
    update();
}



void IBbox::set(int minLat, int minLon, int maxLat, int maxLon)
{
    this->maxLat = maxLat; this->maxLon = maxLon;
    this->minLat = minLat; this->minLon = minLon;
}
