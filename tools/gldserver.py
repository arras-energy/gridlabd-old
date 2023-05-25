"""GridLAB-D server library

Access a GridLAB-D simulation while it is running using REST API.

Examples
--------

1. Start a detached server and get the version:

~~~
$ gridlabd python
>>> from gldserver import GridlabdServer
>>> sim = GridlabdServer(modelname)
>>> print(sim.get_global("version"))
~~~

2. Start an attached server and get the version:

~~~
$ gridlabd python
>>> from gldserver import GridlabdServer
>>> with GridlabdServer(modelname) as sim:
...   print(sim.get_global("version"))
~~~

3. Read an object property as a string

~~~
value = sim.get_property(objname,propname)
~~~

4. Read an object property as a double and print its unit

~~~
value = sim.get_property(objname,propname,astype=GldDouble)
print(value.unit)
~~~

5. Read an object property as a complex and print its real part
value = sim.get_property(objname,propname,astype=GldComplex)
print(value.real)

"""

import sys, os
import requests
import subprocess
import json
import time
import datetime
import math, cmath
import tempfile

#
# Module I/O
#
VERBOSE = False # extra output
QUIET = False # no warning output
SILENT = False # no error output

def verbose(msg):
    """Print a verbose message"""
    if VERBOSE:
        print(f"VERBOSE [server {datetime.datetime.now()}]: {msg}",file=sys.stderr,flush=True)

def error(msg):
    """Print an error message"""
    if not SILENT:
        print(f"ERROR [server {datetime.datetime.now()}]: {msg}",file=sys.stderr,flush=True)

def warning(msg):
    """Print a warning message"""
    if not QUIET:
        print(f"WARNING [server {datetime.datetime.now()}]: {msg}",file=sys.stderr,flush=True)

def exception(msg):
    """Raise an exception"""
    raise GridlabdServerException(msg)

#
# GridLAB-D property types
#
class GldDouble(float):
    """GridLAB-D double property"""

    def __new__(self,value):
        if type(value) is str:
            y = value.split(' ')
            return float.__new__(self,y[0])
        else:
            return float.__new__(x)

    def __init__(self,value):
        if type(value) is str:
            y = value.split(' ')
            float.__init__(y[0])
            self.unit = y[1] if len(y) > 1 else None
        else:
            float.__init__(x)
    
class GldComplex(complex):
    """GridLAB-D complex property"""

    def __new__(self,value,y=None):
        if type(value) is str:
            y = value.split(' ')
            d = y[0][-1]
            if d in "ij":
                return complex.__new__(self,f"{y[0][:-1]}j")
            elif d == 'd':
                z = complex(f"{y[0][:-1]}j")
                return complex.__new__(self,cmath.rect(z.real,z.imag*math.pi/180))
            elif d == 'a':
                z = complex(f"{y[0][:-1]}j")
                return complex.__new__(self,cmath.rect(z.real,z.imag))
            else:
                raise ValueError("invalid complex number")            
            self.unit = y[1] if len(y) > 1 else None
        else:
            return complex.__new__(self,value,y)

    def __init__(self,x,y=None):
        if type(x) is str:
            y = x.split(' ')
            self.unit = y[1] if len(y) > 1 else None
        else:
            complex.__init__(x,y)

    def __str__(self):
        return f"{self.real:+f}{self.imag:+f}j"

#
# Server implementation
#
class GridlabdServerException(Exception):
    """General GridLAB-D server exception"""
    pass

class GridlabdServer:
    """GridLAB-D server"""

    PORT = 6267 # default port to use when connecting
    HOST = "localhost" # default host to use when connecting
    PROTOCOL = "http" # default protocol to use when connecting
    TIMEOUT = 5.0 # default timeout to use when starting/stopping
    RETRYTIME = 0.1 # initial retry time to use when starting/stopping
    LOGFILE = None # file in which to store simulation output (or None, or subprocess.PIPE)

    def __init__(self,*args):
        """Start a detached server"""
        self.status = None
        self.args = args
        if len(args) > 0:
            self.start(*args,detached=True)
        else:
            self.proc = None

    def __del__(self):
        """Stop a detached server"""
        if self.proc:
            verbose(f"sending SIGTERM to process id {self.proc.pid}")
            if self.LOGFILE and not self.LOGFILE.closed:
                self.LOGFILE.close()
            os.kill(self.proc.pid,15)
            self.proc = None

    def __enter__(self,*args):
        """Start an attached server"""
        self.status = None
        self.args = args
        if len(args) > 0:
            self.start(*args)
        else:
            self.proc = None
        return self

    def __exit__(self,*args):
        """Stop an attached server"""
        if self.proc:
            verbose(f"sending SIGTERM to process id {self.proc.pid}")
            if self.LOGFILE and not self.LOGFILE.closed:
                self.LOGFILE.close()
            os.kill(self.proc.pid,15)
            self.proc = None

    def start(self,*args,detached=False):
        """Start the server

        Arguments:

            *args   GridLAB-D command line arguments (may include multiple GLM files)

        Exceptions:

            GridlabdServerException   Unable to start server and get version

            Exception   All other exceptions
        """
        version = self.query("raw","version",onfail=None)
        if version:
            verbose(f"server version '{version}' ok")
            self.proc = None
        else:
            if detached:
                cmd = ["gridlabd","server","start","-D","show_progress=FALSE","-D",f"server_portnum={self.PORT}"]
            else:
                cmd = ["gridlabd","--server","-D","show_progress=FALSE","-D",f"server_portnum={self.PORT}"]
            cmd.extend(args)
            verbose(f"starting {cmd}")
            self.proc = subprocess.Popen(cmd,stdout=self.LOGFILE,stderr=self.LOGFILE)
            timer = 0
            retry = self.RETRYTIME
            while (version:=self.query("raw","version",onfail=None)) == None and timer < self.TIMEOUT:
                time.sleep(retry)
                timer += retry
                retry *= 2 if retry < 5 else 1
            verbose(f"server up in {round(timer,1)} seconds") if version else exception(f"server start timeout after {round(timer,1)} seconds")
        if version:
            verbose(f"connected to {version}")
        else:
            raise GridlabdServerException("unable to connect to server")

    def stop(self):
        """Stop the server normally"""
        verbose("stopping server")
        # if self.proc:
            # if not sys.stdout.closed: sys.stdout.close()
            # if not sys.stderr.closed: sys.stderr.close()
        if self.proc:
            self.proc.terminate()
        self.query("control","stop",onfail=None)
        timer = 0
        retry = self.RETRYTIME
        while (version:=self.query("raw","version",onfail=None)) != None and timer < self.TIMEOUT:
                time.sleep(retry)
                timer += retry
                retry *= 2 if retry < 5 else 1
        if version == None:
            verbose("server stopped")
        else:
            verbose("server ignored stop command")
        if self.proc:
            self.proc.wait()
            self.proc = None

    def query(self,*args,astype=str,onfail=verbose):
        """Send a query to the server REST API"""
        url = f"{self.PROTOCOL}://{self.HOST}:{self.PORT}/{'/'.join(args)}"
        try:
            result = requests.get(url)
            self.status = result.status_code
            verbose(f"query '{url}' --> '{result.text}' (code {result.status_code})")
            return astype(result.text) if result.status_code == 200 else None
        except Exception as err:
            self.status = sys.exc_info()
            if onfail:
                onfail(err)
            return None

    def get_global(self,name,astype=str):
        """Get a global variable from the simulation"""
        return self.query("raw",name,astype=astype)

    def set_global(self,name,value):
        """Set a global variable in the simulation"""
        self.query("raw",f"{name}={value}")

    def get_property(self,obj,name,astype=str):
        """Get an object property"""
        return self.query("raw",obj,name,astype=astype)

    def set_property(self,obj,name,value):
        """Set an object property"""
        self.query("modify",f"{obj}.{name}={value}")

    def get_objects(self,collection):
        """Get a list of objects from a collection"""
        result = self.query("find",collection)
        return [x['name'] for x in json.loads(result)] if result else None

if __name__ == "__main__":

    import unittest
    import tracemalloc
    tracemalloc.start()

    VERBOSE = True
    LOGFILE = None
    TMPFILE = None

    with tempfile.NamedTemporaryFile(suffix=".glm",mode="wt",delete=True) as fh:

        fh.write(f"""// created by {sys.argv} unittest on {datetime.datetime.now()}
#option warn
#ifmissing 123.glm
#model get IEEE/123
#endif
#include "123.glm"
#ifmissing "CA-San_Francisco_Intl_Ap.glm" 
#weather get CA-San_Francisco_Intl_Ap
#endif
#include "CA-San_Francisco_Intl_Ap.glm"
#set run_realtime=1
""")
        TMPFILE = fh.name
        class TestServer(unittest.TestCase):

            def test_detached(self):
                """Verify that server can be started outside a "with" context"""
                fh.seek(0)
                sim = GridlabdServer(fh.name)
                self.assertEqual(len(sim.get_objects("class=load")),85)
                sim.stop()

            def test_attached(self):
                """Verify that changes to loads on a model result in voltage changes"""
                fh.seek(0)
                with GridlabdServer(fh.name) as sim:
                    self.assertEqual(len(sim.get_objects("class=load")),85)
                    self.assertEqual(sim.get_property("node_14","bustype"),"SWING")
                    self.assertEqual(sim.get_property("node_14","voltage_A",astype=GldComplex).real,2401.78)
                    sim.set_property("load_1","constant_power_A",GldComplex(40000,20000))
                    time.sleep(2)
                    self.assertEqual(sim.get_property("load_1","constant_power_A",astype=GldComplex),GldComplex(40000,20000))
                    self.assertEqual(round(sim.get_property("load_1","voltage_A",astype=GldComplex).real,1),2384.8)
                    sim.set_property("load_1","constant_power_A",GldComplex(50000,25000))
                    time.sleep(2)
                    self.assertEqual(sim.get_property("load_1","constant_power_A",astype=GldComplex),GldComplex(50000,25000))
                    self.assertEqual(round(sim.get_property("load_1","voltage_A",astype=GldComplex).real,1),2384.5)
                    
                    sim.stop()

        unittest.main()
    if TMPFILE and os.path.exists(TMPFILE):
        warning(f"temporary file {TMPFILE} was not deleted after test completed")
