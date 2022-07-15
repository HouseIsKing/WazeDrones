import overpass

api = overpass.API(timeout=180)
response = api.get('node["name"="Salt Lake City"]')
print([response["geometry"] for response in response["features"]])