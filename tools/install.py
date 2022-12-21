"""GridLAB-D tool installer

Syntax:

    gridlabd install [OPTIONS ...] ORGANIZATION/REPOSITORY ...

Options:

    --branch|-b=BRANCH   select a branch (default is "main")

"""

import sys, os

BRANCH = "main"
INSTALL = []

for arg in sys.argv[1:]:
    spec = arg.split("=")
    tag,value = (spec[0],True) if len(spec) == 1 else (spec[0],'='.join(spec[1:]))
    if tag in ['-h','--help','help']:
      print(__doc__,file=sys.stdout)
      exit(0)
    elif tag in ['-b','--branch'] and type(value) is str:
        BRANCH = value
    elif tag.startswith('-') or len(arg.split("/")) != 2:
        print(f"ERROR [install]: option '{arg}' is invalid")
        exit(1)
    else:
        INSTALL.append(arg)

if not INSTALL:
    print("Syntax: gridlabd install [OPTIONS ...] ORGANIZATION/REPOSITORY ...",file=sys.stderr)
    exit(1)

for item in INSTALL:
    os.system(f"curl -sL https://raw.githubusercontent.com/{item}/{BRANCH}/install.sh | bash")
