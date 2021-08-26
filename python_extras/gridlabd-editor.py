"""Copyright (C) 2021 Regents of the Leland Stanford Junior University

See https://www.gridlabd.us/ for license, acknowledgments, credits, manuals, documentation, and tutorials.
"""

#
# Python modules
#
import sys, os
import json
import subprocess

#
# Console output
#
def stdout(*msg,file=sys.stdout):
    print(*msg,file=file)

def stderr(*msg,file=sys.stderr):
    print(*msg,file=file)

#
# GridLAB-D link
#
result = subprocess.run("/usr/local/bin/gridlabd --version=json".split(),capture_output=True)
if not result:
    stderr("ERROR: GridLAB-D is not installed on this system")
    quit(-1)
info = json.loads(result.stdout)
# print(info.keys())
version = info['version']
build = info['build_number']
branch = info['branch']
system = info['system']

#
# No GUI on linux yet
#
if os.uname().sysname == "Linux":
    stderr("ERROR: no GUI support on linux systems")
    quit(1)    

#
# Tkinter module
#
try:
    import tkinter as tk
    from tkinter import *
    from tkinter.font import Font
    from tkinter import Menu, messagebox, filedialog, simpledialog, ttk
except Exception as err:
    if os.uname().sysname == 'Darwin':
        stderr(f"ERROR: {err}. Did you remember to run 'brew install python-tk'?",file=sys.stderr)
    else:
        stderr(f"ERROR: {err}. Did you remember to install tkinter support?",file=sys.stderr)
    quit(-1)

try:
    from PIL import Image, ImageTk
except Exception as err:
    if os.uname().sysname == 'Darwin':
        stderr(f"ERROR: {err}. Did you remember to run 'brew install pillow'?",file=sys.stderr)
    else:
        stderr(f"ERROR: {err}. Did you remember to install pillow support?",file=sys.stderr)
    quit(-1)

def TODO(msg="function not implemented yet",context=None):
    if not context:
        context = sys._getframe(1).f_code.co_name
    root.output(f"TODO [{context}]: {msg}")

#
# Platform specifics
#
if sys.platform == "darwin":
    try:
        from Foundation import NSBundle
    except:
        import pip
        pip.main(["install","pyobjc"])
        from Foundation import NSBundle
    bundle = NSBundle.mainBundle()
    if bundle:
        info = bundle.localizedInfoDictionary() or bundle.infoDictionary()
        if info and info['CFBundleName'] == 'Python':
            info['CFBundleName'] = "GridLAB-D"
            info['CFBundleExecutable'] = "GridLAB-D"
            info['CFBundleShortVersionString'] = f"{version}"
            info['CFBundleVersion'] = f"{build} {branch}"
            info['NSHumanReadableCopyright'] = __doc__

#
# Main app startup
#
if __name__ == "__main__":
    root = Tk()
    root.withdraw()
    try:
        ico = Image.open(__file__.replace(".py",".png"))
        photo = ImageTk.PhotoImage(ico)
        root.wm_iconphoto(True, photo)
    except Exception as err:
        print("EXCEPTION:",err)
    messagebox.showinfo("Welcome",
        f"HiPAS GridLAB-D\n{version}-{build} ({branch}) {system}\n\n{__doc__}")
