#include <tiles/png_converter/SVGToPNGConverter.h>

#include "mapoptionsscreen.h"
#include "ui_mapoptionsscreen.h"
#include "params.h"
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileSystemModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include <asterparser.h>
#include <asterparserdynamic.h>
#include <dynamictilesgenerator.h>
#include <math.h>
#include <settingsscreen.h>
#include <QProcess>
#include <vectortextureloader.h>

#include <tiles/OverpassFilter.h>

#include <tiles/svg_converter/OSMToSVGConverter.h>

#include <QRegExpValidator>
#include <vectorosmloader.h>

#define TILE_SIZE 900



MapOptionsScreen::MapOptionsScreen(QWidget *parent)
    : QWidget(parent), ui(new Ui::MapOptionsScreen) {
    dynamicMapDir = DYNAMIC_MAP_DIR;
    ui->setupUi(this);
    ui->vectorButton->setChecked(true);
    ui->latEdit->setText("6.5");
    ui->lonEdit->setText("80.5");
    ui->progressBar->hide();

    QDir dir(dynamicMapDir);
    if (!dir.exists())
        dir.mkpath(dir.absolutePath());

    QRegExpValidator *validatorLat =
        new QRegExpValidator(QRegExp("^-?(90.0)|((([0-8]\\d)|\\d)\\.\\d{6})$"));
    ui->latEdit->setValidator(validatorLat);
    QRegExpValidator *validatorLon =
        new QRegExpValidator(QRegExp("^-?(180.0)|(((1[0-7]\\d)|(\\d?\\d))\\.\\d{6})$"));
    ui->lonEdit->setValidator(validatorLon);

    model = new QFileSystemModel();

    dir.setPath(OSM_DIR);
    model->setRootPath(dir.path());
    model->setReadOnly(true);

    QStringList filters;
    filters << "*.osm"
            << "*.osm.pbf";
    model->setNameFilters(filters);

    connect(ui->loadButton, &QPushButton::clicked, this, &MapOptionsScreen::load);
    connect(ui->dynamicButton, &QRadioButton::clicked, this,
            &MapOptionsScreen::showDynamicRadius);
    connect(ui->vectorButton, &QRadioButton::clicked, this,
            &MapOptionsScreen::showVectorRadius);
    showVectorRadius();
}

UpdateMode MapOptionsScreen::updateMapParams(QString fileOut,
                                                OverpassFilter *filter, QString lat, QString lon) {
    QFile file(fileOut);
    UpdateMode needUpd = UpdateMode::NO_UPDATE;
    bool filterSame = false;
    if (!file.open(QIODevice::ReadOnly) || file.size() == 0) {
        file.close();
        file.open(QIODevice::WriteOnly);
        QTextStream s(&file);
        s << lat << Qt::endl;
        s << lon;
        for (auto e : filter->getIncludedFeatures())
            s << Qt::endl << QString::fromStdString(mf::to_string(e));
        needUpd = UpdateMode::UPDATE_ALL;
    } else {
        QTextStream in(&file);
        QString tlat = in.readLine();
        QString tlon = in.readLine();
        set<mf::MapFeature> includes;
        while (!in.atEnd()) {
            includes.insert(mf::from_string(in.readLine().toStdString()));
        }
        if (includes == filter->getIncludedFeatures()) {
            filterSame = true;
        }
        bool diffrent_center = tlat.compare(lat) != 0 ||
                               tlon.compare(lon) != 0;
        if (diffrent_center || !filterSame) {
            file.close();
            file.open(QIODevice::WriteOnly);
            QTextStream s(&file);
            if (diffrent_center) {
                s << lat << Qt::endl;
                s << lon;
                needUpd = UpdateMode::UPDATE_ALL;
            } else {
                s << tlat << Qt::endl;
                s << tlon;
                needUpd = UpdateMode::UPDATE_FILTER;
            }
            for (auto e : filter->getIncludedFeatures())
                s << Qt::endl << QString::fromStdString(mf::to_string(e));
        }
    }
    file.close();

    return needUpd;
}

/*void MapOptionsScreen::loadStatic(QString _osmDir) {
    if (checkedDist.size() > 0) {

        counter = 0;
        ui->progressBar->show();
        ui->loadButton->hide();
        ui->progressBar->setRange(0, checkedDist.size());
        ui->progressBar->setValue(counter);

        QString outDir = staticMapDir;
        QString contoursFile = "c_out.json";
        QString textureFile = "t_out.json";

        QDir dir(outDir);
        if (!dir.exists())
            dir.mkpath(dir.absolutePath());

        for (auto e : checkedDist) {
            OverpassFilter* filter = SettingsScreen::getFilter(e->text().toInt());
            UpdateMode needUpd = updateMapParams(staticMapDir + e->text() + "_p.txt", filter, ui->latEdit->text(), ui->lonEdit->text());

            if (!QFile::exists(outDir + e->text() + ".png") || needUpd != NO_UPDATE) {
                StaticTilesGenerator *tg =
                    new StaticTilesGenerator(OSM_DIR, outDir, contoursFile, textureFile, e->text(), filter, TILE_SIZE);

                connect(tg, &QThread::finished, this, &MapOptionsScreen::msg);

                if (needUpd==UPDATE_ALL || !QFile::exists(outDir + e->text() + "_" + textureFile) ||
                    !QFile::exists(outDir + e->text() + "_" + contoursFile) ) {
                    AsterParser *p =
                        new AsterParser(ASTER_DIR, outDir, contoursFile, textureFile,
                                        QString::number(TILE_SIZE), ui->latEdit->text(),
                                        ui->lonEdit->text(), e->text());

                    connect(p, &QThread::finished, tg, &StaticTilesGenerator::startPainting);
                    p->start();
                } else {
                    tg->startPainting();
                }
            } else
                msg();
        }
    }
}*/

void MapOptionsScreen::loadDynamic(QString _osmDir) {
    if (checkedDist.size() > 0) {
        counter = 0;
        int osmFilesCount = 0;
        QDirIterator *it = new QDirIterator(_osmDir, QStringList("*.osm"),
                                            QDir::Files | QDir::NoDotAndDotDot,
                                            QDirIterator::Subdirectories);
        while (it->hasNext()) {
            osmFilesCount++;
            it->next();
        }

        ui->progressBar->show();
        ui->loadButton->hide();
        ui->progressBar->setRange(0, checkedDist.size() * osmFilesCount);
        ui->progressBar->setValue(counter);


        QFile f(dynamicMapDir + "params.txt");
        if(f.open(QFile::WriteOnly)){
            QTextStream ss(&f);
            ss<<ui->latEdit->text()<<" "<<ui->lonEdit->text()<<Qt::endl;
            QString tilesDir = dynamicMapDir;
            ss<<tilesDir;
            f.close();
        }

        DynamicTilesGenerator* tg = new DynamicTilesGenerator(dynamicMapDir, TILE_SIZE, checkedDist);
        connect(tg, &DynamicTilesGenerator::tileGenerated, this, &MapOptionsScreen::msg);
        tg->start();
    }
}

void MapOptionsScreen::loadVector(QString _osmDir)
{
    if (checkedDist.size() > 0) {
        counter = -1;

        __TIME__

        QFile f(VECTOR_MAP_PARAMS_FILE);
        bool same = false;
        if(f.exists()){
            f.open(QFile::ReadOnly);
            QTextStream ss(&f);
            QStringList sl = ss.readLine().split(" ");
            QString dist = ss.readLine();
            dist = ss.readLine();
            f.close();
            same = (sl[0] == ui->latEdit->text() && sl[1] == ui->lonEdit->text() && fabs(dist.toDouble() - checkedDist[0]->text().toDouble()) <= 1e-7);

        }
        if(!same && f.open(QFile::WriteOnly)){
            QTextStream ss(&f);
            ss<<ui->latEdit->text()<<" "<<ui->lonEdit->text()<<Qt::endl;
            ss<<_osmDir<<endl;
            ss<<checkedDist[0]->text();
            f.close();
        }

        ui->progressBar->show();
        ui->loadButton->hide();
        ui->progressBar->setRange(0, 0);
        ui->progressBar->setValue(counter);

        if(!same){
            VectorOsmLoader * ol = new VectorOsmLoader(ui->latEdit->text().toDouble(), ui->lonEdit->text().toDouble(), checkedDist[0]->text().toDouble());
            VectorTextureLoader * tl = new VectorTextureLoader(TMP_VEC_TEXTURE,ASTER_DIR, ol->getBox());
            connect(tl, &VectorTextureLoader::finished, ol, &VectorOsmLoader::load);
            connect(ol, &VectorOsmLoader::finished, this, &MapOptionsScreen::msg);
            tl->start();
        }
        else msg();
    }
}

void MapOptionsScreen::showDynamicRadius() {
    for (auto e : ui->distance->findChildren<QCheckBox*>())
        e->setCheckState(Qt::Unchecked);
    ui->cb_001->hide();
    ui->cb_002->hide();
    ui->cb_005->hide();
    ui->cb_020->hide();
    ui->cb_025->hide();
    ui->cb_035->hide();
    ui->cb_070->hide();
    ui->cb_050->show();
    ui->cb_100->show();
    ui->cb_100->setCheckState(Qt::Checked);
}


void MapOptionsScreen::showVectorRadius()
{
    for (auto e : ui->distance->findChildren<QCheckBox*>())
        e->setCheckState(Qt::Unchecked);

    ui->cb_001->show();
    ui->cb_002->show();
    ui->cb_005->show();
    ui->cb_005->setCheckState(Qt::Checked);
    ui->cb_020->hide();
    ui->cb_025->hide();
    ui->cb_035->hide();
    ui->cb_050->hide();
    ui->cb_070->hide();
    ui->cb_100->hide();
}

void MapOptionsScreen::load() {
    QString _osmDir = OSM_DIR;

    checkedDist.clear();
    for (auto e : ui->distance->findChildren<QCheckBox *>()) {
        if (e->isChecked()) {
            checkedDist.push_back(e);
        }
    }

    if (ui->dynamicButton->isChecked()) {
        loadDynamic(_osmDir);
    }
    if(ui->vectorButton->isChecked()){
        loadVector(_osmDir);
    }
}

MapOptionsScreen::~MapOptionsScreen() { delete ui; }

void MapOptionsScreen::msg() {
    counter++;
    ui->progressBar->setValue(counter);
    if (counter == ui->progressBar->maximum()) {
        if (ui->dynamicButton->isChecked()) {
            emit showDynamic(checkedDist);
        }
        if (ui->vectorButton->isChecked()) {
            emit showVector(checkedDist);
        }
        ui->progressBar->hide();
        ui->loadButton->show();
    }
}
