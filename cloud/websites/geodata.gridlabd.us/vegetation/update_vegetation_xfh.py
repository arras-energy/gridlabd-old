import os
import requests
import math
from PIL import Image
import numpy
import json
import cfo
from osgeo import gdal
import rasterio
import matplotlib.pyplot as plt
import numpy as np
import boto3


default_options = {
    "year" : 2020,
}
default_config = {
    "maximum_image_size" : 2000000000,
}

access_key = " "
secret_access_key = " "
upload_file_bucket = "geodata.gridlabd.us"
client = boto3.client('s3', aws_access_key_id = access_key, 
                      aws_secret_access_key = secret_access_key, region_name='us-west-1')


def get_imagename(layer,pos,scale=0.1):
    """Get the image name for a location

    ARGUMENTS

        layer (str)         The vegetation data layer, e.g., "cover", "height", "base"
        pos (float,float)   The latitude and longitude of the location
        scale (float)       The scale at which the name is generated (default 0.1)

    RETURNS

        tifname (str)       The name of the image tile used
    """
    lat = pos[0]/scale
    lon = pos[1]/scale
    if lat < 0:
        lat = f"{int(-np.rounf(lat))}S"
    elif lat > 0:
        lat = f"{int(np.round(lat))}N"
    else:
        lat = "0"
    if lon < 0:
        lon = f"{int(-np.round(lon))}W"
    elif lon > 0:
        lon = f"{int(np.round(lon))}E"
    else:
        lon = "0"
    return f"{layer}/{lat}_{lon}"

def get_imagedata(layer, pos, scale, repourl, cachedir,
        year = default_options['year'],
        maximum_image_size = default_config['maximum_image_size'],
        not_again = False):
    """Get the image data for a location

    ARGUMENTS

        layer (str)         The vegetation layer desired, i.e., "cover", "height", "base"
        pos (float,float)   The latitude and longitude of the location

    RETURNS

        tifname (str)       The name of the image tile used
        vegetation (nparray) The vegetation data from the image
    """
    tifname = get_imagename(layer,pos,scale)
    os.makedirs(f"{cachedir}/{year}/{layer}",exist_ok=True)
    srcname = f"{repourl}/{year}/{tifname}.tif"
    dstname = f"{cachedir}/{year}/{tifname}.tif"
    if not os.path.exists(dstname):
        print(f"Downloading {year}/{tifname}.tif...",flush=True)
        response = requests.get(srcname,stream=True)
        if response.status_code != 200:
            raise Exception(f"'GET {srcname}' --> HTTP error code {response.status_code}")
        try:
            with open(dstname,"wb") as fh:
                for chunk in response.iter_content(chunk_size=10*1024*1024):
                    if chunk:
                        fh.write(chunk)
        except:
            os.remove(dstname)

    source = rasterio.open(dstname)
    data = source.read(1).astype(float)
    print(f"{year}/{layer}/{tifname} missing data: ",sum(sum(data == source.nodata)))
    data[data == source.nodata] = 0
    
#     _plot(data, "img data")
    
    os.remove(dstname)
    img = Image.fromarray(np.uint8(data))
    img.save(dstname)
    
    if layer == "base":
        dstsize = 3 * 3600 * scale
    elif layer == "cover" or layer == "height":
        dstsize = 9 * 3600 * scale
    try:
        if len(data) != dstsize:
            print(f"WARNING: {year}/{tifname}.tif is not the correct size for scale={scale}, size={len(data)}x{len(data[0])}",flush=True)
    except:
        os.remove(dstname)
        if not_again:
            print(f"ERROR: {year}/{tifname}.tif is not a valid vegetation data file",flush=True)
            raise
        else:
            pass
            return get_imagedata(layer,pos,scale,repourl,cachedir,year,maximum_image_size,notagain=True)
    return data

def get_geojson(pos, cachedir):
    geojson = {
        'type': 'FeatureCollection',
        "name": "vegetation_tile",
        "crs": {
            "type": "name", 
            "properties": { 
                "name": "urn:ogc:def:crs:OGC:1.3:CRS84" 
            } 
        },
        "features": [ { 
            "type": "Feature", 
            "geometry": { 
                "type": "Polygon", 
                "coordinates": [ [ [ pos[1], pos[0] ], [ pos[1], pos[0]+0.1 ], 
                                  [ pos[1]+0.1, pos[0]+0.1 ], [ pos[1]+0.1, pos[0] ], [ pos[1], pos[0] ] ] ] 
            } 
        }]
    }
    vector = os.path.join(cachedir, "vegetation_tile.geojson")
    with open(vector, 'w') as f:
        f.write(json.dumps(geojson))
    f.close()
    return vector

def get_image(layer, pos, forest, name, cachedir):
    vector = get_geojson(pos, cachedir)
    if layer == "base":
        ca_search = forest.search(geography='California', metric='CanopyBaseHeight', year = default_options['year'], resolution=10)
        options = gdal.WarpOptions(
            creationOptions = ["COMPRESS=DEFLATE", "TILED=YES", "BIGTIFF=YES", "NUM_THREADS=ALL_CPUS"],
            cutlineDSName = vector,
            cropToCutline = True,
            resampleAlg = gdal.GRA_Average,
            dstSRS = "EPSG:3857",
#             dstSRS = "EPSG:4326",
            width=1080,
            height=1080)
    elif layer == "cover":
        ca_search = forest.search(geography='California', metric='CanopyCover', year = default_options['year'], resolution=3)
        options = gdal.WarpOptions(
            creationOptions = ["COMPRESS=DEFLATE", "TILED=YES", "BIGTIFF=YES", "NUM_THREADS=ALL_CPUS"],
            cutlineDSName = vector,
            cropToCutline = True,
            resampleAlg = gdal.GRA_Average,
            dstSRS = "EPSG:3857",
#             dstSRS = "EPSG:4326",
            width=3240,
            height=3240)
    elif layer == "height":
        ca_search = forest.search(geography='California', metric='CanopyHeight', year = default_options['year'], resolution=3)
        options = gdal.WarpOptions(
            creationOptions = ["COMPRESS=DEFLATE", "TILED=YES", "BIGTIFF=YES", "NUM_THREADS=ALL_CPUS"],
            cutlineDSName = vector,
            cropToCutline = True,
            resampleAlg = gdal.GRA_Average,
            dstSRS = "EPSG:3857",
#             dstSRS = "EPSG:4326",
            width=3240,
            height=3240)
    
    cs_vegetation = ca_search[0]
    input_file = forest.fetch(cs_vegetation, gdal=True)
    output_file = f"/usr/local/share/gridlabd/geodata/vegetation/2020/{name}.tif"

    warp = gdal.Warp(output_file, input_file, options=options)
    warp.FlushCache()

def _plot(img, title=None):
    plt.figure(figsize=(8,8), dpi=100)
    vmin = np.nanpercentile(img, 1)
    vmax = np.nanpercentile(img, 99)
    if vmin < 0:
        vmin = 0
    imag_map = plt.imshow(img, vmin=vmin, vmax=vmax, cmap=plt.cm.cividis)
#     plt.grid()
    plt.title(title)
    colorbar = plt.colorbar(imag_map)
    plt.show()

# CA lat_range = range(32,43), lon_range = range(-125,-114)
coordinates = ["41N_125W", "41N_124W", "41N_123W", "41N_122W", "41N_121W", 
               "40N_125W", "40N_124W", "40N_123W", "40N_122W", "40N_121W",
               "39N_125W", "39N_124W", "39N_123W", "39N_122W", "39N_121W", 
               "38N_124W", "38N_123W", "38N_122W", "38N_121W", "38N_120W", "38N_119W",
               "37N_124W", "37N_123W", "37N_122W", "37N_121W", "37N_120W", "37N_119W", "37N_118W",
               "36N_122W", "36N_121W", "36N_120W", "36N_119W", "36N_118W", "36N_117W", "36N_116W",
               "35N_122W", "35N_121W", "35N_120W", "35N_119W", "35N_118W", "35N_117W", "35N_116W", "35N_115W",
               "34N_121W", "34N_120W", "34N_119W", "34N_118W", "34N_117W", "34N_116W", "34N_115W",
               "33N_119W", "33N_118W", "33N_117W", "33N_116W", "33N_115W",
               "32N_118W", "32N_117W", "32N_116W", "32N_115W"]
forest = cfo.api()
for layer in ["base", "cover", "height"]:
    for coordinate in coordinates:
        lats = np.linspace(float(coordinate[0:2]), float(coordinate[0:2])+1.0, 10, endpoint=False)
        lons = np.linspace(-float(coordinate[4:7]), -float(coordinate[4:7])+1.0, 10, endpoint=False)
        for lat in lats:
            for lon in lons:
                name = get_imagename(layer,pos=(lat,lon),scale=0.1)
                print(name)
                if not os.path.exists(f"/usr/local/share/gridlabd/geodata/vegetation/2020/{name}.tif"):
                    print(f"Downloading " + name)
                    get_image(layer, (lat,lon), forest, name,
                              cachedir = "/usr/local/share/gridlabd/geodata/vegetation")
                    tile = get_imagedata(layer, (lat,lon), scale = 0.1,
                                         repourl = "http://geodata.gridlabd.us/vegetation",
                                         cachedir = "/usr/local/share/gridlabd/geodata/vegetation")
                
                srcname = f"/usr/local/share/gridlabd/geodata/vegetation/2020/{name}.tif"
                dstname = f"vegetation/2020/{name}.tif"
                client.upload_file(srcname, upload_file_bucket, dstname, 
                                   ExtraArgs={'ACL':'bucket-owner-full-control'})



























