from haversine import haversine
from math import sin,cos,radians,atan2,degrees
import json
import sys

ORIGIN_POINT = (32.0853, 34.7818) #Origin point - (lat, lon)
BUILDING_HEIGHT = 10

def get_json(file='../tel_aviv.geojson'):
    """
    Get json file from the given path.
    @param file: path to the json file
    @return: json file
    """
    
    with open(file,encoding="utf8") as f:
        data = json.load(f)
    return data

def get_xy(lat,lon):
    """
    @param lat: latitude
    @param lon: longitude
    @return: x,y coordinates
    """
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
    return x,y

def parse_json(data):
    """
    @param data: json file
    @print: list of buildings with their coordinates and height
    """
    buildings = data['features']
    print(len(buildings))
    for building in buildings:
        if building['geometry']['type'] != 'Polygon': continue

        print(len(building['geometry']['coordinates'][0]))

        cords_list = building['geometry']['coordinates'][0]
        for cord in cords_list:
            print(*get_xy(cord[1],cord[0]))

        # if no key: height - use default value
        if 'height' in building['properties']:
            BUILDING_HEIGHT = building['properties']['height']
        elif 'building:levels' in building['properties']:
            BUILDING_HEIGHT = float(building['properties']['building:levels']) * 3  # 3 meters per level

        print(BUILDING_HEIGHT) 

if __name__ == '__main__':
    sys.stdout = open('../tel_aviv.txt', 'w')
    data = get_json()
    parse_json(data)
    sys.stdout.close()