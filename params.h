#ifndef PARAMS_H
#define PARAMS_H

#include <QHash>
#include <QList>
#include <QString>
#include "tiles/MapFeatures.h"
#include <QDebug>
using namespace map_features;

static time_t stime = time(NULL);

#define __TIME__ {\
    qDebug()<<__PRETTY_FUNCTION__<<" "<<time(NULL) - stime;\
    stime = time(NULL);\
}



#define TILE_STEP  0.5
#define TILE_STEP_PREC  1
#define PATTERNS_FILE "../world_map/data/static_style/patterns.txt"
#define STYLE_SVG  "../world_map/data/static_style/styles/style100km.css"
#define STYLE_DIR "../world_map/data/static_style/"
#define VEC_STYLE "../world_map/data/vector_style/style.json"

#define TMP_VEC_OSM "vec.osm"
#define TMP_VEC_TEXTURE "vec_tex.json"
#define TEMP_FILE_DIR "tmp/"
#define OSM_DIR "osm/"
#define PBF_DIR "planet/"
#define ASTER_DIR "aster/"
#define PARSED_ASTER "asterParsed/"
#define DYNAMIC_MAP_DIR "dynamic_map/"


#define DYNAMIC_ASTER_PARSER_PY "../world_map/python/dparser.py"
#define VEC_ASTER_PARSER_PY "../world_map/python/vparser.py"
#define PYTHON "../world_map/python/venv/bin/python"

#define ASTER_URL "https://gdemdl.aster.jspacesystems.or.jp/download/"

#define WORLD_MAP_PNG "../cuba_map/data/cuba.png"
#define WORLD_PBF_DIR "../world_map/planet/"

#define VECTOR_MAP_PARAMS_FILE "vec_params.txt"
#define FONT_LOC "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"

#define BACKGROUND_COLOR_GL 0x00,0x00,0x02,0x00
#define MIN_GRID_LINES_COUNT 2
#define MIN_SVG_ITEM_SQUARE 5
#define PLANET_TILES_STEP 5
#define TEXT_SIZE 12
#define GRID_COLOR 0x96,0x96,0x96
#define DEFAULT_TILE_SIZE 450

static QHash<QString, int> dynamicTilesPrec = {{"100", 0},
                                        {"50", 1}};
static QHash<QString, QString> dynamicTilesSize = {{"100", "1"},
                                            {"50", "0.5"}};

#define MAP_FEATURES_FILE "mf.json"
const QList<MapFeature> f_100km = {Aerialway, Aeroway, Barrier, Boundary, Highway, Landuse, Leisure, ManMade, Military, Natural, Power, Railway, Route, Telecom, Water, Waterway};
const QList<MapFeature> f_70km = {Aerialway, Aeroway, Amenity, Barrier, Boundary, Highway, Landuse, Leisure, ManMade, Military, Natural, Power, Railway, Route, Telecom, Water, Waterway};
const QList<MapFeature> f_50km = {Aerialway, Aeroway, Amenity, Barrier, Boundary, Highway, Landuse, Leisure, ManMade, Military, Natural, Power, Railway, Route, Telecom, Water, Waterway};
const QList<MapFeature> f_35km = {Aerialway, Aeroway, Amenity, Barrier, Boundary, Highway, Landuse, Leisure, ManMade, Military, Natural, Power, Railway, Route, Telecom, Water, Waterway};
const QList<MapFeature> f_25km = {Aerialway, Aeroway, Amenity, Barrier, Boundary, Building, Geological, Highway, Landuse, Leisure, ManMade, Military, Natural, Office, Place, Power, Railway, Route, Telecom, Water, Waterway};
const QList<MapFeature> f_20km = {Aerialway, Aeroway, Amenity, Barrier, Boundary, Building, Geological, Highway, Landuse, Leisure, ManMade, Military, Natural, Office, Place, Power, Railway, Route, Telecom, Water, Waterway};
const QList<MapFeature> f_5km = {Aerialway, Aeroway, Amenity, Barrier, Boundary, Building, Craft, Emergency, Geological, Highway, Landuse, Leisure, ManMade, Military, Natural, Office, Place, Power, Railway, Route, Telecom, Water, Waterway};

#endif // PARAMS_H
