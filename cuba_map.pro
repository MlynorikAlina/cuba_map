QT       += core gui network opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lzip -lcairo -lgio-2.0 -lgobject-2.0 -lglib-2.0 -lcairo -lrsvg-2 -lglut -lglfw -lGL -lGLEW -ljsoncpp -lpugixml -lftgl -LGLU

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
    dynamicscreen.cpp \
    dynamictilesgenerator.cpp \
    main.cpp \
    mainwindow.cpp \
    mapgl.cpp \
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
    vectormapgl.cpp

HEADERS += \
    asterdownloader.h \
    asterparser.h \
    clickdetector.h \
    dynamicmapgl.h \
    dynamicscreen.h \
    dynamictilesgenerator.h \
    mainwindow.h \
    mapgl.h \
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
    vectormapgl.h

FORMS += \
    dynamicscreen.ui \
    mainwindow.ui \
    mapscreen.ui \
    parameterswindow.ui \
    settingsscreen.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    data/download_sources.json \
    data/map_parameters.json \
    vec_style/style.json
