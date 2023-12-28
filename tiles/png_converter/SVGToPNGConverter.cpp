//
// Created by alina-mlynorik on 8/3/23.
//

#include "SVGToPNGConverter.h"

#include <librsvg/rsvg.h>
#include <filesystem>
#include <iostream>

void SVGToPNGConverter::convert(const QString &svgFile, const QString &outputDir, const QString &outputFileName) {
    GError *error = NULL;
    GFile *file = g_file_new_for_path (svgFile.toStdString().c_str());
    RsvgHandle *handle = rsvg_handle_new_from_gfile_sync (file, RSVG_HANDLE_FLAGS_NONE, NULL, &error);

    if(!filesystem::exists(outputDir.toStdString()))
        filesystem::create_directories(outputDir.toStdString());

    if (!handle)
    {
        printf ("could not load: %s", error->message);
        exit (1);
    }

    if (!rsvg_handle_render_document (handle, cr, &viewport, &error))
    {
        printf ("could not render: %s", error->message);
        exit (1);
    }


    /* Write a PNG file */

    if (cairo_surface_write_to_png (surface, (outputDir + outputFileName).toStdString().c_str()) != CAIRO_STATUS_SUCCESS)
    {
        printf ("could not write output file");
        exit (1);
    }
    /* Free our memory and we are done! */
    g_object_unref (handle);
    g_object_unref (file);
}

SVGToPNGConverter::SVGToPNGConverter(double width, double height) {
    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
    cr = cairo_create (surface);

    viewport = {
        .x = 0.0,
        .y = 0.0,
        .width = width,
        .height = height,
    };

}

SVGToPNGConverter::~SVGToPNGConverter() {
    cairo_destroy (cr);
    cairo_surface_destroy (surface);
}

void SVGToPNGConverter::setViewport(double size) {
    viewport = {
        .x = 0.0,
        .y = 0.0,
        .width = size,
        .height = size,
    };
}

void SVGToPNGConverter::setShift(int shift_x, int shift_y) {
    cairo_surface_set_device_offset(surface,-shift_x,-shift_y);
}
