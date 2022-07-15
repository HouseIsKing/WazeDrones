import overpass

api = overpass.API(timeout=180)
response = api.get('area["name"="Tel Aviv"]', verbosity='geom')
print(response)