QT       += core gui network opengl svg

#set PROJ_DATA = "../cuba_map_data/python/Lib/site-packages/rasterio/proj_data/"

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14
QMAKE_CXXFLAGS += -g
CONFIG += console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lzip -lcairo -lgio-2.0 -lgobject-2.0 -lglib-2.0  -lrsvg-2 -lglut -lglfw -lGL -lGLEW -ljsoncpp -lpugixml -lftgl -LGLU

INCLUDEPATH += /usr/include/librsvg-2.0 \
    /usr/include/cairo \
    /usr/include/glib-2.0 \
    /usr/lib/x86_64-linux-gnu/glib-2.0/include \
    /usr/include/gdk-pixbuf-2.0 \
    /usr/include/freetype2



SOURCES += \
    asterdownloader.cpp \
    asterparser.cpp \
    clickdetector.cpp \
    dynamicmapgl.cpp \
    dynamictilesgenerator.cpp \
    main.cpp \
    mainwindow.cpp \
    mapgl.cpp \
    maploader.cpp \
    mapscreen.cpp \
    network.cpp \
    osmloader.cpp \
    osmtilesgenerator.cpp \
    parameterswindow.cpp \
    settingsscreen.cpp \
    staticmapgl.cpp \
    statictilesgenerator.cpp \
    tiles/GeoInfo.cpp \
    tiles/Geometry.cpp \
    tiles/GraphicPrimitives.cpp \
    tiles/MapFeatures.cpp \
    tiles/OverpassFilter.cpp \
    tiles/coastline/CoastlinePainter.cpp \
    tiles/elevation/EleContoursProcessor.cpp \
    tiles/elevation/GrahamMVO.cpp \
    tiles/parser/OSMFileParser.cpp \
    tiles/png_converter/SVGToPNGConverter.cpp \
    tiles/svg_converter/OSMData.cpp \
    tiles/svg_converter/OSMParser.cpp \
    tiles/svg_converter/OSMToSVGConverter.cpp \
    tiles/svg_converter/SVGPainter.cpp \
    tilesselector.cpp \
    tilesselectorscreen.cpp \
    vectormapgl.cpp

HEADERS += \
    asterdownloader.h \
    asterparser.h \
    clickdetector.h \
    dynamicmapgl.h \
    dynamictilesgenerator.h \
    mainwindow.h \
    mapgl.h \
    maploader.h \
    mapscreen.h \
    network.h \
    osmloader.h \
    osmtilesgenerator.h \
    parameterswindow.h \
    params.h \
    settingsscreen.h \
    staticmapgl.h \
    statictilesgenerator.h \
    tiles/GeoInfo.h \
    tiles/Geometry.h \
    tiles/GraphicPrimitives.h \
    tiles/MapFeatures.h \
    tiles/OverpassFilter.h \
    tiles/coastline/CoastlinePainter.h \
    tiles/elevation/EleContoursProcessor.h \
    tiles/elevation/GrahamMVO.h \
    tiles/parser/OSMFileParser.h \
    tiles/png_converter/SVGToPNGConverter.h \
    tiles/svg_converter/OSMData.h \
    tiles/svg_converter/OSMParser.h \
    tiles/svg_converter/OSMToSVGConverter.h \
    tiles/svg_converter/SVGPainter.h \
    tilesselector.h \
    tilesselectorscreen.h \
    vectormapgl.h

FORMS += \
    mainwindow.ui \
    parameterswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
