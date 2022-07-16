import overpass
import json

api = overpass.API(timeout=180)
query="""{{geocodeArea:Tel Aviv, Israel}}->.searchArea; 
  node["building"](area.searchArea);
  way["building"](area.searchArea);    
  relation["building"](area.searchArea);"""
response = api.get(query)
print(json.dumps(response, indent=2))
with open('telAviv.json', 'w') as outfile:
    json.dump(response, outfile)