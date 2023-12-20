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
    transform = dataset.transform * dataset.transform.scale(
        (dataset.width / imgdata.shape[-1]),
        (dataset.height / imgdata.shape[-2]))
    return imgdata[0]


def processCropped(dataset, bounds, SIZE):
    wind = rasterio.windows.from_bounds(bounds[0], bounds[1], bounds[2], bounds[3], transform=dataset.transform)

    imgdata = dataset.read(window=wind, out_shape=(dataset.count,
                                                   SIZE,
                                                   SIZE),
                           resampling=rasterio.enums.Resampling.average)
    return imgdata[0]

MASK_VALUE = -1000
prec=0
tileLen = 1



def getCoordinates(filename):
    lat = int(filename[1:3])
    if(filename[0]=='S'):
        lat*=-1
    lon = int(filename[4:7])
    if(filename[3]=='W'):
        lon*=-1

    lat+=90
    lon+=180
    return lat, lon


fmt1 = "{:02d}"
fmt2 = "{:03d}"
def getFileName(lat,lon):
    n=''
    if(lat>=0):
        n+='N'
    else:
        n+='S'
    n+=fmt1.format(abs(lat))
    if(lon>=0):
        n+='E'
    else:
        n+='W'
    n+=fmt2.format(abs(lon))
    n+='.tif'
    return n



def parseAster(asterDir, outFile, SIZE, lonMin, latMin, lonMax, latMax):
    zeros = np.zeros((3601,3601))
    tex = None
    for i in range(math.floor(latMax), math.floor(latMin)-1, -1):
        line = None
        print(i)
        for j in range(math.floor(lonMin),math.floor(lonMax)+1):
            print(j)
            fn = asterDir + getFileName(i,j)
            texture = zeros
            img = zeros
            if os.path.exists(fn):
                with rasterio.open(fn) as dataset:
                    img, transform = processAll(dataset)
                #print(texture.shape)

            geotransform = (j, 1./3600, 0., i+1,  0., -1./3600)
            transform = ~Affine.from_gdal(*geotransform)

            j_s,i_e = map(int,transform*(lonMin,latMin))
            j_e,i_s = map(int,transform*(lonMax,latMax))

            timg = img[max(0,i_s):max(0,i_e),max(0,j_s):max(0,j_e)]
            
            if line is None:
                line = timg
            else:
                line = np.concatenate((line, timg),axis=1)

        if tex is None:
            tex = line
        else:
            tex = np.concatenate((tex,line),axis=0)

    h, w = tex.shape
    kwargs = {
            'driver': 'GTiff', 
            'dtype': 'int16',
            'nodata': None, 
            'width': w, 
            'height': h, 
            'count': 1, 
            'crs': rasterio.crs.CRS.from_epsg(4326), 
            'transform': Affine(1/w, 0.0, lonMin,
       0.0, 1/h, latMin)}

    with rasterio.open('temp.tif', 'w', **kwargs) as dst:
        dst.write_band(1, tex.astype(rasterio.float32))
    
    with rasterio.open('temp.tif') as dataset:
        img = processResampled(dataset, SIZE)
        tex = np.where(img>0,1,MASK_VALUE).astype(np.float32)

        imgMask = np.ma.masked_array(tex, mask=(tex != MASK_VALUE))
        f = open(outFile,'w')
        f.write("{\"box\" : [" + str(latMin) + ',' + str(lonMin) + ',' + str(latMax) + ',' + str(lonMax) + "]}")
        for geom, val in rasterio.features.shapes(tex, mask=imgMask.mask):
            f.write('\n{\"coordinates\" : ' + str(
                    [[x for x in el] for el in geom['coordinates'][0]]) + '}')
        
        f.close()


parseAster(sys.argv[1], sys.argv[2], int(sys.argv[3]), float(sys.argv[4]), float(sys.argv[5]), float(sys.argv[6]), float(sys.argv[7]))