"""Convert Schneider ION meter data to GLM players

SYNOPSIS

Shell:

    bash$ gridlabd -D csv_load_options="-f schneider -t player [-O KEY:VALUE] INPUTCSV

GLM:

    #input "INPUTCSV" -f schneider -t player [OPTIONS ...]

Python:

    >> convert(inputfile=INPUTCSV,outputfile=OUTPUTGLM[,options=OPTIONS])

DESCRIPTION 

The Schneider ION meter converter creates a CSV player file and a GLM object
for each source in INPUTCSV.  By default the CSV player file name is
generated from the source name, and the GLM file is generated from the
INPUTCSV file name. 

The format of the input CSV file is standardized for the Schneider ION meter
output and must contain the following columns:

- `Timestamp UTC`: The UTC timestamp at which the value is sampled.

- `Source`: The source meter name from which the value is sampled.

- `Measurement`: The measurement which the value represents. These must
  include `Real Power` and `Reactive Power` to generate a valid player file.

- `Unit`: The unit of the measurement. The `Real Power` unit is used to determine
the scaling for player values. Valid units are `W`, `kW`, and `MW`.  If the unit
is not recognized, then the `player_scale` option must be set.

- `Value`: The value that is sampled.

OPTIONS

General options:

- `verbose`: Enable verbose output (default `False`).

- `debug`: Enable debug output (default `False`).

- `quiet`: Enable quiet mode (default `True`).

- `warning`: Disable warning mode (default `False`).

Input CSV file options:

- `csv_index`: Specify the CSV index column (default `Timestamp UTC`).

- `csv_units`: Specify the CSV unit column (default `Unit`).

- `csv_source`: Specify the CSV source column (default `Source`).

- `csv_measurement`: Specify the CSV measurement column (default `Measurement`).

- `csv_value`: Specify the CSV value column (default `Value`).

- `csv_compression`: Specify the file compression format (default `None`). See
  pandas.read_csv() for valid options.

- `csv_real_power`: Specify the real power measurement name (default `Real
  Power`).

- `csv_reactive_power`: Specify the reactive power measurement name
  (default `Reactive Power`).

Output CSV player options:

- `player_header`: Enable output of header row (default `False`)

- `player_index`: Specify the index column in the player file
  (default `timestamp`).

- `player_suffix`: Specify the player file suffix (default `_player.csv`).

- `player_date_format`: Specify the format of the timestamp column (default `%s`)

- `player_complex_format`: Specify the complex power output format (default `
  {0.real:g}{0.imag:+g}j`).

- `player_scale`: Specify the scale of power values (default `auto`)

Output GLM player options:

- `glm_with_load`: Include load in the GLM output (default `False`).

- `glm_with_meter`: Include meter in the GLM output (default `False`). This
  automatically includes a load object as well.

- `glm_power`: Specify the power property to play (default `constant_power_{0}`)
  where parameter 0 is the phase.

- `glm_load_map`: Specify the GLM load mapping file to use for creating load
  and meter objects.  Column 0 must be the input CSV basename. Valid columns are
  `parent`, `phases`, and `nominal_voltage`.

Property defaults:

- `default_phases`: Specify the default phases for load and meter objects.

- `default_voltage`: Specify the default voltage for load and meter objects.


"""
import os, sys
import pandas as pd

default_options = dict(
    verbose = False,
    debug = False,
    quiet = False,
    warning = True,

    csv_index = "Timestamp UTC",
    csv_units = "Unit",
    csv_source = "Source",
    csv_measurement = "Measurement",
    csv_value = "Value",
    csv_compression = None,
    csv_real_power = "Real Power",
    csv_reactive_power = "Reactive Power",

    player_header = False,
    player_index = "timestamp",
    player_suffix = "_player.csv",
    player_date_format = "%s",
    player_complex_format = "{0.real:g}{0.imag:+g}j",
    player_scale = 'auto',

    glm_power = "constant_power_{0}",
    glm_load_map = None,
    glm_with_load = False,
    glm_with_meter = False,

    default_phases = "ABC",
    default_voltage = "480 V",
)

def convert(inputfile,outputfile,options=default_options):

    for key,value in default_options.items(): 
        if not key in options: 
            options[key] = value

    if options["verbose"]:
        print(f"VERBOSE [csv-schneider2glm-player]: converting '{inputfile}' to '{outputfile}' with options = {options}",file=sys.stderr)

    global PATHNAME
    PATHNAME = os.path.splitext(outputfile)[0]

    global BASENAME
    BASENAME = os.path.basename(PATHNAME)
    if options['csv_compression']:
        BASENAME = os.path.basename(BASENAME)

    global PLAYERFILE
    PLAYERFILE = PATHNAME+options['player_suffix']

    global GLMFILE
    GLMFILE = BASENAME + ".glm"

    global LOADMAP
    try:

        if options['glm_load_map']:
            LOADMAP = pd.read_csv(options['glm_load_map'],index_col=0).to_dict('index')
            if options['debug']:
                print(f"DEBUG [csv-schneider2glm-player]: LOADMAP = \n{LOADMAP}",file=sys.stderr)

        csv = pd.read_csv(inputfile,
            parse_dates=[options['csv_index']],
            compression=options['csv_compression']).dropna(axis=0)

        for SOURCE in csv["Source"].unique():
            process(csv[csv[options['csv_source']]==SOURCE],options)
    except:

        if options['debug']:
            
            raise
        
        elif not options["quiet"]:
        
            etype, evalue, etrace = sys.exc_info()
            efile = os.path.basename(etrace.tb_frame.f_code.co_filename)
            eline = etrace.tb_lineno
            print(f"ERROR [csv-schneider2glm-player]: {etype.__name__} {evalue} ({efile}:{eline})",file=sys.stderr)
            return False

    return True

def process(csv,options):

    data = csv.pivot(
        index = options['csv_index'],
        columns = [options['csv_measurement'],options['csv_units']],
        values = options['csv_value'])
    data.index.name = options['player_index']
    columns = write_csv(data,options)
    write_glm(data,columns,options)

def write_csv(data,options):

    global PLAYERFILE
    P = data.loc[:,options['csv_real_power']]
    Q = data.loc[:,options['csv_reactive_power']]
    PU = P.columns[0]
    QU = Q.columns[0]
    if options['player_scale'] == 'auto':
        if P.columns[0] == 'W':
            scale = 1.0
        elif P.columns[0] == 'kW':
            scale = 1e3
        elif P.columns[0] == 'MW':
            scale = 1e6
        else:
            raise Exception(f"unit '{P.columns[0]}' is not recognized, use option 'player_scale' to set power scale")
    else:
        try:
            scale = float(options['player_scale'])
        except:
            raise Exception(f"player scale '{options['player_scale']}' is not a valid float")

    S = list(map(lambda x: options['player_complex_format'].format(x),(P[PU] + Q[QU]*1j) / 3.0 * scale))

    player = pd.DataFrame({
        options['glm_power'].format('A'):S,
        options['glm_power'].format('B'):S,
        options['glm_power'].format('C'):S,
        }, index = data.index)
    player.to_csv(PLAYERFILE,
        header = bool(options['player_header']),
        index = options['player_index'],
        date_format = options['player_date_format'])
    return player.columns

def write_glm(data,columns,options):

    global GLMFILE
    global PLAYERFILE
    global BASENAME
    global LOADMAP

    phases = options['default_phases']
    try: phases = LOADMAP[BASENAME]['phases']
    except: pass

    nominal_voltage = options['default_voltage']
    try: nominal_voltage = LOADMAP[BASENAME]['nominal_voltage']
    except: pass

    parent = BASENAME
    try: parent = LOADMAP[BASENAME]['parent']
    except: pass


    with open(GLMFILE,"w") as glm:
        glm.write(f'''// generated by {" ".join(sys.argv)}
module tape;
''')
        if options['glm_with_meter'] or options['glm_with_load']:
            glm.write(f'''module powerflow;
''')

        if options['glm_with_meter']:
            glm.write(f'''object meter
{{
    parent "{parent}";
    phases "{phases}";
    nominal_voltage {nominal_voltage};
    object load
    {{
        phases "{phases}";
        nominal_voltage {nominal_voltage};
        object player
        {{
            property "{','.join(columns)}";
            file "{PLAYERFILE}";
        }};
    }};
}}
''')
        
        elif options['glm_with_load']:
            glm.write(f'''object load
{{
    parent "{parent}";
    phases "{phases}";
    nominal_voltage {nominal_voltage};
    object player
    {{
        property "{','.join(columns)}";
        file "{PLAYERFILE}";
    }};
}}
''')
        else:
            glm.write(f'''object player
{{
    parent "{parent}";
    property "{','.join(columns)}";
    file "{PLAYERFILE}";
}}
''')
