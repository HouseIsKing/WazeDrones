from haversine import haversine
from math import sin,cos,radians,atan2,degrees
import json
import sys

ORIGIN_POINT = (32.0853, 34.7818) #Origin point - (lat, lon)
BUILDING_HEIGHT = 1
PARK_HEIGHT = 0.1


def get_json(file):
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


def parse_buildings_json(data):
    """
    @param data: json file
    @print: list of buildings with their coordinates and height
    """

    sys.stdout = open('../WazeDrones/Data/buildings_tel_aviv.txt', 'w')
    left_point = (32.0476023,34.7885795)
    right_point = (32.0823978,34.8238885)
    left_point_xy = get_xy(*left_point)
    right_point_xy = get_xy(*right_point)
    print(*left_point_xy)
    print(*right_point_xy)

    buildings = data['features']
    print(len(buildings))
    for building in buildings:
        if building['geometry']['type'] != 'Polygon': continue

        cords_list = building['geometry']['coordinates'][0]
        print(len(cords_list))

        for cord in cords_list:
            xy = get_xy(cord[1],cord[0])
            print(round(xy[0],7),round(xy[1],7))

        # if no key: height - use default value
        if 'height' in building['properties']:
            DEFAULT_BUILDING_HEIGHT = building['properties']['height']
        elif 'building:levels' in building['properties']:
            DEFAULT_BUILDING_HEIGHT = float(building['properties']['building:levels']) * 3  # 3 meters per level

        print(DEFAULT_BUILDING_HEIGHT)

    sys.stdout.close()


def parse_parks_json(data):
    """
    @param data: json file
    @print: list of parks with their coordinates and height
    """

    sys.stdout = open('../WazeDrones/Data/parks_tel_aviv.txt', 'w')

    parks = data['features']
    print(len(parks))

    for park in parks:
        if park['geometry']['type'] == 'Point': continue

        cords_list = park['geometry']['coordinates'][0]
        print(len(cords_list))

        for cord in cords_list:
            xy = get_xy(cord[1],cord[0])
            print(round(xy[0],7),round(xy[1],7))
    
        print(PARK_HEIGHT)
    sys.stdout.close()


def parse_roads_json(data):
    """
    @param data: json file
    @print: list of roads with their coordinates
    """

    sys.stdout = open('../WazeDrones/Data/roads_tel_aviv.txt', 'w')

    roads = data['features']
    print(len(roads))

    for road in roads:

        cords_list = road['geometry']['coordinates']
        if len(cords_list) <= 2: continue
        print(len(cords_list))

        for cord in cords_list:
            xy = get_xy(cord[1],cord[0])
            print(round(xy[0],7),round(xy[1],7))

        print('0')
    sys.stdout.close()


if __name__ == '__main__':
    
    data = get_json('../buildings_tel_aviv.geojson')
    parse_buildings_json(data)
    data = get_json('../parks_tel_aviv.geojson')
    parse_parks_json(data)
    data = get_json('../roads_tel_aviv.geojson')
    parse_roads_json(data)