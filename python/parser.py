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


def processCropped(dataset, bounds, resample_factor):
    wind = rasterio.windows.from_bounds(bounds[0], bounds[1], bounds[2], bounds[3], transform=dataset.transform);
    windTransform = dataset.window_transform(wind)

    imgdata = dataset.read(window=wind, out_shape=(dataset.count,
                                                   int(wind.height * resample_factor),
                                                   int(wind.width * resample_factor)),
                           resampling=rasterio.enums.Resampling.average)

    transform = windTransform * windTransform.scale(
        (wind.width / imgdata.shape[-1]),
        (wind.height / imgdata.shape[-2]))
    return imgdata, transform


def smoothContours(img, ratio):
    o = np.ones(img.shape)
    t = (img[0] + np.pad(img[0,1:,:], ((0,1),(0,0)), mode='constant') +np.pad(img[0,:-1,:], ((1,0),(0,0)), mode='constant')+np.pad(img[0,:,1:], ((0,0),(0,1)), mode='constant')
       +np.pad(img[0,:,:-1], ((0,0),(1,0)), mode='constant'))/(o[0] + np.pad(o[0,1:,:], ((0,1),(0,0)), mode='constant') +np.pad(o[0,:-1,:], ((1,0),(0,0)), mode='constant')
       +np.pad(o[0,:,1:], ((0,0),(0,1)), mode='constant')+np.pad(o[0,:,:-1], ((0,0),(1,0)), mode='constant'))
    t = (np.round(t / ratio, 0) * ratio).astype(np.float32)
    img[0] = np.where(img[0]>0, t, MASK_VALUE)


def maskImage(img, contoursDist, ratio):
    o = np.ones(img.shape)
    s = np.zeros(img.shape)
    so = np.zeros(img.shape)

    for i,j in np.ndindex((contoursDist,contoursDist)):
       s[0] += np.pad(img[0,i:,j:], ((0,i),(0,j)), mode='constant') 
       so[0] += np.pad(o[0,i:,j:], ((0,i),(0,j)), mode='constant')

    timg = s/so
    rp = (np.round(timg / ratio, 0) * ratio).astype(np.float32)
    rp = rp[:,::contoursDist,::contoursDist]
    smoothContours(rp, ratio)

    q,n,m = rp.shape

    for i in range(0, n-1):
        for j in range(0, m-1):
            if rp[0][i][j] != MASK_VALUE:
                if rp[0][i][j+1] != MASK_VALUE and rp[0][i][j] != rp[0][i][j+1]:
                    rp[0][i][j+1] = MASK_VALUE
                if rp[0][i+1][j] != MASK_VALUE and rp[0][i][j] != rp[0][i+1][j]:
                    rp[0][i+1][j] = MASK_VALUE
                              
    
    q,n,m = img.shape
    img[0] = np.repeat(np.repeat(rp[0],contoursDist, axis=1),contoursDist,axis=0)[:n,:m]          
                    


MASK_VALUE = -1000
prec=0
tileLen = 1


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


contour_frac_p = [50, 25, 15, 10]
dist_p = [100,50,25,20]
contour_dist_p = [15, 15, 15, 15]


KM_IN_1_DEG_LAT = 40008.55 / 360
def _kmIn1degLon(lat):
    lonrad = lat * math.pi / 180.0;
    return math.cos(lonrad) * 40075.696 / 360;


def km2lat(dist):
    return dist / KM_IN_1_DEG_LAT;


def km2long(dist, point_lat):
    return dist / _kmIn1degLon(point_lat);

class Coord:

    def __init__(self, lat, lon, dist):
        self.lat_min = lat - km2lat(dist)
        self.lon_min = lon - km2long(dist, lat)
        self.lat_max = lat + km2lat(dist)
        self.lon_max = lon + km2long(dist, lat)



def parseAster(aster_dir, outDir, contoursOutFile,textureOutFile, SIZE, lat, lon, dist, contour_frac):
    arr = [None]
    zeros = np.zeros((3601,3601))
    coords = []
    coords.append(Coord(lat, lon, dist))

    
    for lat in range(math.floor(coords[0].lat_max), math.floor(coords[0].lat_min)-1, -1):
        line = [None]
        #print(lat)
        for lon in range(math.floor(coords[0].lon_min), math.floor(coords[0].lon_max) + 1):
            fn = aster_dir +getFileName(lat,lon)
            print(lat,lon)
            
            img = zeros
            if os.path.exists(fn):
                with rasterio.open(fn) as dataset:
                    img, transform = processAll(dataset)

            geotransform = (lon, 1./3600, 0., lat+1,  0., -1./3600)
            transform = ~Affine.from_gdal(*geotransform)
            print(transform)

            j_s,i_e = map(int,transform*(coords[0].lon_min,coords[0].lat_min))
            j_e,i_s = map(int,transform*(coords[0].lon_max,coords[0].lat_max))

            timg = img[max(0,i_s):max(0,i_e),max(0,j_s):max(0,j_e)]
            #print(lat,lon, i, timg.shape)
            if line[0] is None:
                line[0] = timg
            else:
                line[0] = np.concatenate((line[0],timg),axis=1)
            

        if arr[0] is None:
            arr[0] = line[0]
        else:
            arr[0] = np.concatenate((arr[0],line[0]),axis=0)


    h, w = arr[0].shape
    kwargs = {
            'driver': 'GTiff', 
            'dtype': 'int16',
            'nodata': None, 
            'width': w, 
            'height': h, 
            'count': 1, 
            'crs': rasterio.crs.CRS.from_epsg(4326), 
            'transform': Affine(1/w, 0.0, coords[0].lon_min,
       0.0, 1/h, coords[0].lat_min)}

    t = time.time()
    #print(arr[i].shape)
    tf = str(dist) + '_temp.tif'
    with rasterio.open(tf, 'w', **kwargs) as dst:
        dst.write_band(1, arr[0].astype(rasterio.float32))
    
    with rasterio.open(tf) as dataset:
        img = processResampled(dataset, SIZE)
        
        print(img.shape, time.time()-t)
        t = time.time()
        texture = np.where(img>0,1,MASK_VALUE).astype(np.float32)
        imgMask = np.ma.masked_array(texture, mask=(texture != MASK_VALUE))
        f = open(outDir + str(int(dist)) + '_' + textureOutFile,'w')
        f.write("{\"box\" : [" + str(coords[0].lat_min) + ',' + str(coords[0].lon_min) + ',' + str(coords[0].lat_max) + ',' + str(coords[0].lon_max) + "]}")
        for geom, val in rasterio.features.shapes(texture, mask=imgMask.mask):
            f.write('\n{\"coordinates\" : ' + str(
                    [[x for x in el] for el in geom['coordinates'][0]]) + '}')
        f.close()


        rp = processResampled(dataset, int(SIZE/15))
        rp = (np.ceil(rp / contour_frac) * contour_frac).astype(np.float32)
        
        q,n,m = rp.shape
        for k in range(0, n-1):
            for j in range(0, m-1):
                if rp[0][k][j] != MASK_VALUE:
                    if rp[0][k][j+1] != MASK_VALUE and rp[0][k][j] != rp[0][k][j+1]:
                        rp[0][k][j+1] = MASK_VALUE
                    if rp[0][k+1][j] != MASK_VALUE and rp[0][k][j] != rp[0][k+1][j]:
                        rp[0][k+1][j] = MASK_VALUE

                    
        img = np.repeat(np.repeat(rp,15, axis=2),15,axis=1)
        imgMask = np.ma.masked_array(img, mask=(img != MASK_VALUE))
        f = open(outDir + str(int(dist)) + '_' + contoursOutFile,'w')
        #f.write("{\"box\" : [" + str(coords[0].lat_min) + ',' + str(coords[0].lon_min) + ',' + str(coords[0].lat_max) + ',' + str(coords[0].lon_max) + "]}")
        for geom, val in rasterio.features.shapes(img, mask=imgMask.mask):
            f.write('{\"ele\" : ' + str(val) + ', \"coordinates\" : ' + str(
                    [[x for x in el] for el in geom['coordinates'][0]]) + '}\n')
        f.close()
        print(img.shape, time.time()-t)


parseAster(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], int(sys.argv[5]), float(sys.argv[6]), float(sys.argv[7]), float(sys.argv[8]), float(sys.argv[9]))