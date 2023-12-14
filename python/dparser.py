import rasterio
import rasterio.features
import rasterio.warp
import numpy as np
import time
import math
import os
from scipy import ndimage
from affine import Affine
import sys

def processAll(dataset):
    imgdata = dataset.read(1)
    return imgdata, dataset.transform


def processResampled(dataset, SIZE):
    imgdata = dataset.read(out_shape=(dataset.count,
                                      SIZE,
                                      SIZE),
                           resampling=rasterio.enums.Resampling.average)
    return imgdata

def processCropped(dataset, bounds, SIZE):
    wind = rasterio.windows.from_bounds(bounds[0], bounds[1], bounds[2], bounds[3], transform=dataset.transform)

    imgdata = dataset.read(window=wind, out_shape=(dataset.count,
                                                   SIZE,
                                                   SIZE),
                           resampling=rasterio.enums.Resampling.average)
    return imgdata


MASK_VALUE = -1000
prec=0
tileLen = 1



def getCoordinates(filename):
    lat = float(filename[1:3])
    if(filename[0]=='S'):
        lat*=-1
    lon = float(filename[4:7])
    if(filename[3]=='W'):
        lon*=-1

    return lat, lon


contour_frac_p = [50, 25, 15, 10]
dist_p = [100,50,25,20]
contour_dist_p = [15, 15, 15, 15]


class Coord:

    def __init__(self, lat, lon, frac):
        self.lat_min = lat
        self.lon_min = lon
        self.lat_max = lat + frac
        self.lon_max = lon + frac




def parseAster(aster_dir, outDir, SIZE, frac, flp):
    fmt = "{:." + str(flp) + "f}"
    for entry in os.scandir(aster_dir):
        if(entry.is_file()):
            fn = entry.name
            mlat, mlon = getCoordinates(fn)
            with rasterio.open(entry.path) as dataset:
                bounds = [round(e) for e in dataset.bounds]
                while (bounds[3] - mlat >= frac):
                    mlon = bounds[0]
                    while (bounds[2] - mlon >= frac):
                        #print(mlat, mlon)
                        ofn = outDir + fmt.format(mlat+90) + '_' + fmt.format(mlon+180) + '.json'
                        if(not os.path.exists(ofn)):
                            b = [mlon,mlat,mlon+frac,mlat+frac]
                            img = processCropped(dataset,b, SIZE)
                            texture = np.where(img>0,1,MASK_VALUE).astype(np.float32)
                            imgMask = np.ma.masked_array(texture, mask=(texture != MASK_VALUE))
                            f = open(ofn,'w')
                            f.write("{\"box\" : [" + fmt.format(b[1]) + ',' + fmt.format(b[0]) + ',' + fmt.format(b[3]) + ',' + fmt.format(b[2]) + "]}")
                            for geom, val in rasterio.features.shapes(texture, mask=imgMask.mask):
                                f.write('\n{\"coordinates\" : ' + str(
                                        [[x for x in el] for el in geom['coordinates'][0]]) + '}')
                            f.close()
                        mlon+=frac
                    mlat+=frac


parseAster(sys.argv[1], sys.argv[2], int(sys.argv[3]), float(sys.argv[4]), int(sys.argv[5]))