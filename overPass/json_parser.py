from haversine import haversine
from math import sin,cos,radians,atan2,degrees
import json
import sys

ORIGIN_POINT = (32.0853, 34.7818) #Origin point - (lat, lon)
BUILDING_HEIGHT = 10

def get_json(file='../tel_aviv.geojson'):
    with open(file,encoding="utf8") as f:
        data = json.load(f)
    return data

def get_xy(lat,lon):
    point = (lat, lon)
    distance = haversine(ORIGIN_POINT, point, unit='m')

    long1 = radians(ORIGIN_POINT[0])
    long2 = radians(point[0])
    lat1  = radians(ORIGIN_POINT[1])
    lat2  = radians(point[1])
    dLon  = (long2 - long1)

    y = sin(dLon) * cos(lat2)
    x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon)

    brng = atan2(y, x)

    brng = degrees(brng)
    brng = (brng + 360) % 360
    brng = 360 - brng                   # count degrees counter-clockwise from North 
    y = sin(radians(brng)) * distance
    x = cos(radians(brng)) * distance
    print('x = ' + str(x))
    print('y = ' + str(y))

if __name__ == '__main__':
    # stdout to new file
    sys.stdout = open('../tel_aviv.txt', 'w')
    data = get_json()
    for building in data['features']:
        if building['geometry']['type'] != 'Polygon':
            continue
        for cord in building['geometry']['coordinates'][0]:
            print("lat:",cord[1],"lon:",cord[0])
            get_xy(cord[1],cord[0])
        # if no key: height - use default value
        if 'height' in building['properties']:
            BUILDING_HEIGHT = building['properties']['height']
        print("height:",BUILDING_HEIGHT)
        print('-----------------------------------------------------\n')
    sys.stdout.close()