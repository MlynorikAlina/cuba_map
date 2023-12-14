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
    return imgdata[0], transform


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
    zeros = np.zeros((SIZE,SIZE))
    tex = None
    for i in range(latMin,latMax+1):
        line = None
        for j in range(lonMin,lonMax+1):
            fn = asterDir + getFileName(i,j)
            texture = zeros
            if os.path.exists(fn):
                print(fn)
                with rasterio.open(fn) as dataset:
                    img, transform = processResampled(dataset, SIZE)
                    texture = np.where(img>0,1,MASK_VALUE).astype(np.float32)
                    #print(texture.shape)
            
            if line is None:
                line = texture
            else:
                line = np.concatenate((line, texture),axis=1)

        if tex is None:
            tex = line
        else:
            tex = np.concatenate((line,tex),axis=0)
        print(tex.shape)

    imgMask = np.ma.masked_array(tex, mask=(tex != MASK_VALUE))
    print(imgMask.shape)
    f = open(outFile,'w')
    
    geotransform = (lonMin, 1./ SIZE, 0., latMax+1,  0., -1./SIZE)
    transform = Affine.from_gdal(*geotransform)
    for geom, val in rasterio.features.shapes(tex, transform=transform, mask=imgMask.mask):
        f.write('{\"coordinates\" : ' + str(
                [[x for x in el] for el in geom['coordinates'][0]]) + '}\n')
    
    f.close()


parseAster(sys.argv[1], sys.argv[2], int(sys.argv[3]), int(sys.argv[4]), int(sys.argv[5]), int(sys.argv[6]), int(sys.argv[7]))