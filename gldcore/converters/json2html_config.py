"""Configuration file for JSON to HTML converter"""

#
# Default values for options
#
icon_prefix = "glyphicon" # --glyphs
zoomlevel = 'auto' # --zoomlevel
show = False # --show
tiles = "OpenStreetMap" # --tiles
cluster_ok = True # --cluster
zoomtoggle = False

#
# Tile options
#
map_tiles = {
    "street" : "OpenStreetMap",
    "terrain" : "Stamen Terrain",
    "lineart" : "Stamen Toner",
}

#
# Icon glyphs for object classes
#
icons = {"fa":
    {
        "substation" : "sitemap",
        "node" : "circle",
        "load" : "chevron-circle-down",
        "triplex_load" : "angle-double-down",
        "regulator" : "adjust",
        "switch" : "square",
        "transformer" : "squarespace",
        "meter" : "clock",
        "capacitor" : "hockey-puck",
        "inverter" : "chevron-circle-up",
        "office" : "building",
        "apartment" : "hotel",
        "house" : "home",
        "evcharger" : "charging-station",
        "industrial" : "industry",
        "parking" : "parking",
        "solar" : "solar-panel",
        "battery" : "battery-half",
        "energy-storage" : "battery-empty",
        "weather" : "wind",
        "climate" : "sun",
    },
    "glyphicon":
    {
        "substation" : "glyphicon-th-list",
        "node" : "glyphicon-record",
        "load" : "glyphicon-circle-arrow-down",
        "triplex_load" : "glyphicon-triangle-bottom",
        "regulator" : "glyphicon-ban-circle",
        "switch" : "glyphicon-stop",
        "transformer" : "glyphicon-link",
        "meter" : "glyphicon-dashboard",
        "capacitor" : "glyphicon-oil",
        "inverter" : "chevron-circle-up",
        "office" : "glyphicon glyphicon-th",
        "apartment" : "glyphicon-th-large",
        "house" : "glyphicon-home",
        "evcharger" : "glyphicon-flash",
        "industrial" : "glyphicon-home",
        "parking" : "glyphicon-home",
        "solar" : "glyphicon-home",
        "battery" : "glyphicon-home",
        "energy-storage" : "glyphicon-modal-window",
        "weather" : "glyphicon-certificate",
        "climate" : "glyphicon-certificate",
    },
}

#
# Node colors are based on voltage
#
voltage_colors = {
    "zero" : "black",
    "low" : "blue",
    "normal" : "green",
    "high" : "red"
}

# 
# Voltage thresholds (per unit nominal_voltage)
#
low_voltage = 0.95
high_voltage = 1.05

#
# Link colors are based on current relative to continuous and emergency ratings
#
current_colors = {
    "zero" : "black",
    "normal" : "green",
    "continuous" : "orange",
    "emergency" : "red",
}
