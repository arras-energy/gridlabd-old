"""Generate 1/10 deg tile from 1 deg tiles for North America vegetation data"""
import os
import requests
import math
from PIL import Image
import numpy

GLD_ETC = os.getenv("GLD_ETC")

default_options = {
    "year" : 2020,
}
default_config = {
    "maximum_image_size" : 2000000000,
}

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
        lat = f"{-math.floor(lat)}S"
    elif lat > 0:
        lat = f"{math.floor(lat)}N"
    else:
        lat = "0"
    if lon < 0:
        lon = f"{-math.floor(lon)}W"
    elif lon > 0:
        lon = f"{math.floor(lon)}E"
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
    Image.MAX_IMAGE_PIXELS = maximum_image_size
    img = Image.open(dstname)
    data = numpy.array(img)
    try:
        if len(data) != 3600/scale:
            print(f"WARNING: {year}/{tifname}.tif is not the correct size for scale={scale}, size={len(data)}x{len(data[0])}",flush=True)
    except:
        os.remove(dstname)
        if not_again:
            print(f"ERROR: {year}/{tifname}.tif is not a valid vegetation data file",flush=True)
            raise
        else:
            pass
            return get_imagedata(layer,pos,scale,repourl,cachedir,year,maximum_image_size,notagain=True)
    return tifname, data

lat_range = range(25,50)
lon_range = range(-135,-65)
for layer in ["base","cover","height"]:
    for lat in lat_range:
        for lon in lon_range:
            name = get_imagename(layer,pos=(lat,lon),scale=1.0)
            if not os.path.exists(f"{GLD_ETC}/gridlabd/geodata/vegetation/2020/{name}.tif"):
                continue
            print(f"Processing {layer}...",flush=True)
            name,tile = get_imagedata(layer, pos = (37.420457,-122.204568), scale = 1.0,
                    repourl = "http://geodata.gridlabd.us/vegetation",
                    cachedir = "{GLD_ETC}/gridlabd/geodata/vegetation")
            width = len(tile[0])
            height = len(tile)
            missing = 0
            for row in range(height):
                missing += len(numpy.where(tile[row]==255))
            if missing > 0:
                print(f"WARNING: missing data found in {name}.tif")
            for row in range(10):
                for col in range(10):
                    # print(f"row={row},col={col}")
                    slat = round(lat+row/10,1)
                    slon = round(lon+col/10,1)
                    # print(f"slat={slat},slon={slon}")
                    name = get_imagename(layer, pos=(slat+0.05,slon+0.05), scale=0.1)
                    print(f"Saving {name}.tif...",flush=True)
                    left = int(col*width/10)
                    right = int(left+width/10+1)
                    bottom = int(row*height/10)
                    top = int(bottom+height/10+1)
                    # print(f"{name}: left={left}, right={right}, bottom={bottom}, top={top}")
                    img = Image.fromarray(tile[bottom:top,left:right])
                    img.save(f"{GLD_ETC}/gridlabd/geodata/vegetation/2020/{name}.tif")
