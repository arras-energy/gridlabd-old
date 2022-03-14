"""Configuration file for JSON to HTML converter"""

#
# Default values for options
#
icon_prefix = 'fa' # --glyphs
zoomlevel = 'auto' # --zoomlevel
show = False # --show
tiles = 'OpenStreetMap' # --tiles
cluster_ok = True # --cluster

#
# Control options
mouseposition = 'bottomright' # enables mouse position in lower
geocoder = 'topright' # enable the geocoder search tool
search = None # enables to search tool (cannot be used with --cluster option)
measurecontrol = None # enable the measurement tool
zoomtoggle = None # enable the zoom toggle control

#
# Tile options
#
map_tiles = {
    'street' : 'OpenStreetMap',
    'terrain' : 'Stamen Terrain',
    'lineart' : 'Stamen Toner',
}

#
# Icon glyphs for object classes
#
icons = {'fa': # See https://fontawesome.com/v4.7.0/icons/ for available icons
    {
        'substation' : 'sitemap',
        'node' : 'circle',
        'pole' : 'fa-flash',
        'load' : 'chevron-circle-down',
        'triplex_load' : 'angle-double-down',
        'regulator' : 'adjust',
        'switch' : 'square',
        'transformer' : 'random',
        'meter' : 'dashboard',
        'capacitor' : 'plug',
        'inverter' : 'chevron-circle-up',
        'office' : 'building',
        'apartment' : 'hotel',
        'house' : 'home',
        'evcharger' : 'leaf',
        'industrial' : 'industry',
        'parking' : 'car',
        'solar' : 'table',
        'battery' : 'battery-half',
        'energy-storage' : 'battery-empty',
        'weather' : 'cloud',
        'climate' : 'sun-o',
    },
    'glyphicon': # See https://bootstrapdocs.com/v3.2.0/docs/components/ for available icons
    {
        'substation' : 'glyphicon-th-list',
        'node' : 'glyphicon-record',
        'pole' : 'glyphicon-flash',
        'load' : 'glyphicon-circle-arrow-down',
        'triplex_load' : 'glyphicon-triangle-bottom',
        'regulator' : 'glyphicon-ban-circle',
        'switch' : 'glyphicon-stop',
        'transformer' : 'glyphicon-link',
        'meter' : 'glyphicon-dashboard',
        'capacitor' : 'glyphicon-tower', 
        'inverter' : 'glyphicon-collapse-up',
        'office' : 'glyphicon glyphicon-th',
        'apartment' : 'glyphicon-th-large',
        'house' : 'glyphicon-home',
        'evcharger' : 'glyphicon-leaf',
        'industrial' : 'glyphicon-home',
        'parking' : 'glyphicon-home',
        'solar' : 'glyphicon-home',
        'battery' : 'glyphicon-home',
        'energy-storage' : 'glyphicon-unchecked',
        'weather' : 'glyphicon-certificate',
        'climate' : 'glyphicon-certificate',
    },
}

#
# Node colors are based on voltage
#
voltage_colors = {
    'zero' : 'black',
    'low' : 'blue',
    'normal' : 'green',
    'high' : 'red'
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
    'zero' : 'black',
    'normal' : 'green',
    'continuous' : 'orange',
    'emergency' : 'red',
}

#
# Pole colors are based on value of status property
#
pole_colors = {
    'OK' : 'black',
    'FAILED' : 'red',
}
#
# Hidden properties
#
hidden_properties = ['rank','clock','rng_state','flags']

#
# Show command used to open the browser
#
show_command = 'open'