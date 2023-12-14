//
// Created by alina-mlynorik on 8/3/23.
//

#ifndef MAPDOWNLOADER_SVGTOPNGCONVERTER_H
#define MAPDOWNLOADER_SVGTOPNGCONVERTER_H

#include <string>
#include <cairo/cairo.h>
#include <librsvg/rsvg.h>

using namespace std;

class SVGToPNGConverter {
private:
    cairo_surface_t *surface;
    cairo_t *cr;
    RsvgRectangle viewport;
public:
    SVGToPNGConverter(double width, double height);

    virtual ~SVGToPNGConverter();

    void convert(const string &svgFile, const string &outputDir, const string &outputFileName);

    void setViewport(double size);
    void setShift(int shift_x, int shift_y);
};


#endif //MAPDOWNLOADER_SVGTOPNGCONVERTER_H
