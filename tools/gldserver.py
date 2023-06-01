"""GridLAB-D server library

Access a GridLAB-D simulation while it is running using REST API.

Examples
--------

1. Start a detached server and get the version:

    $ gridlabd python
    >>> from gldserver import GridlabdServer
    >>> sim = GridlabdServer(modelname)
    >>> print(sim.get_global("version"))

2. Start a server in a context and get the version:

    $ gridlabd python
    >>> from gldserver import GridlabdServer
    >>> with GridlabdServer(modelname) as sim:
    ...   print(sim.get_global("version"))

3. Read an object property as a string

    value = sim.get_property(objname,propname)

4. Read an object property as a double and print its unit

    value = sim.get_property(objname,propname,astype=GldDouble)
    print(value.unit)

5. Read an object property as a complex and print its real part

    value = sim.get_property(objname,propname,astype=GldComplex)
    print(value.real)

6. Set a property

    sim.set_property(objname,propname,value)
"""

import sys, os
import requests
import subprocess
import json
import time
import datetime
import math, cmath
import tempfile
import datetime, pytz

#
# Module I/O
#
VERBOSE = False # extra output
QUIET = False # no warning output
SILENT = False # no error output

def verbose(msg):
    """Print a verbose message"""
    if VERBOSE:
        print(f"VERBOSE [gldserver {datetime.datetime.now()}]: {msg}",file=sys.stderr,flush=True)

def error(msg):
    """Print an error message"""
    if not SILENT:
        print(f"ERROR [gldserver {datetime.datetime.now()}]: {msg}",file=sys.stderr,flush=True)

def warning(msg):
    """Print a warning message"""
    if not QUIET:
        print(f"WARNING [gldserver {datetime.datetime.now()}]: {msg}",file=sys.stderr,flush=True)

def exception(msg):
    """Raise an exception"""
    raise GridlabdServerException(msg)

#
# GridLAB-D property types
#
class GldDouble(float):
    """GridLAB-D double property

    Properties:

        unit (str)      Unit in which value is represented
    """

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
    """GridLAB-D complex property

    Properties:

        unit (str)      Unit in which value is represented
    """

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

class GldTimestamp(datetime.datetime):
    """GridLAB-D timestamp property

    Properties:

        tz ()
    """
    tz = pytz.timezone("UTC")
    url = None
    fmt = "%Y-%m-%d %H:%M:%S %Z"
    def __new__(self,*args):
        dt = self.parse(*args)
        return datetime.datetime.__new__(self,dt.year,dt.month,dt.day,dt.hour,dt.minute,dt.second,tzinfo=dt.tzinfo)

    def __init__(self,*args):
        dt = self.parse(*args)
        datetime.datetime.__init__(dt.year,dt.month,dt.day,dt.hour,dt.minute,dt.second,tzinfo=dt.tzinfo,fold=dt.fold)

    def format(self,fmt=None):
        return self.strftime(fmt if fmt else self.fmt)

    @classmethod
    def parse(self,*args):
        if len(args) == 1:
            if type(args[0]) is str:
                url = f"{self.url}/util/convert_to_timestamp/{args[0].replace(' ','%20')}"
                # breakpoint()
                reply = requests.get(url)
                if reply.status_code == 200:
                    return datetime.datetime.fromtimestamp(int(reply.text),tz=self.tz)
                return None
            elif type(args[0]) is int:
                return datetime.datetime.fromtimestamp(args[0],tz=self.tz)
        else:
            return datetime.datetime(*args)
        exception("invalid date/time type")

#
# Server implementation
#
class GridlabdServerException(Exception):
    """General GridLAB-D server exception"""
    pass

class GridlabdServer:
    """GridLAB-D server"""

    HOST = "localhost" # default host to use when connecting
    PROTOCOL = "http" # default protocol to use when connecting
    TIMEOUT = 5.0 # default timeout to use when starting/stopping
    RETRYTIME = 0.1 # initial retry time to use when starting/stopping
    LOGFILE = None # file in which to store simulation output (or None, or subprocess.PIPE)

    def __init__(self,*args,detached=True):
        """Start a server

        Properties:

            args        Command options for GridLAB-D

            detached    Specify whether server remains attached to this
                        process (default True)

        Exceptions:

            GridlabdServerException   Unable to start server
        """
        self.status = None
        self.args = args
        self.port = None
        if len(args) > 0:
            self.start(*args,detached=detached)

    def __del__(self):
        """Cleanup

        Exceptions:

            GridlabdServerException   Unable to stop server
        """
        self.stop()

    def __enter__(self,*args):
        """Start a server in context

        Properties:

            args    Command options for GridLAB-D
        """
        return self

    def __exit__(self,*args):
        """Stop a server in context

        Exceptions:

            GridlabdServerException   Unable to stop server
        """
        self.stop()    

    def start(self,*args,detached=False):
        """Start the server

        Arguments:

            *args       GridLAB-D command line arguments

            detached    Specify whether the server should remain attached to
                        this process (default False) 

        Exceptions:

            GridlabdServerException   Unable to start server

            Exception   All other exceptions
        """
        if (status:=self.wait("RUNNING")) == "RUNNING":
            if detached:
                exception(f"cannot attach to process on port {self.port}")
            version = self.getversion()
            verbose(f"server version '{version}' ok")
            self.proc = None
        elif status:
            exception(f"server up but not running (status={status})")
        else:
            cmd = ["gridlabd","server","start"]
            if not self.port is None:
                cmd.extend(["-p",str(self.port)])
            if detached:
                cmd.append("--detach")
            cmd.extend(["-D","show_progress=FALSE"])
            cmd.extend(args)
            verbose(f"starting {cmd}")
            self.proc = subprocess.Popen(cmd,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
            time.sleep(1)
            self.port = self.proc.stdout.readline().strip().decode('utf-8')
            verbose(f"started ok on port {self.port}")
            if self.wait("RUNNING") != "RUNNING":
                exception(f"server start timeout")
            version = self.getversion()
            verbose(f"server version {version} up")
        GldTimestamp.tz = pytz.timezone(self.get_global("timezone_locale"))
        GldTimestamp.url = f"http://localhost:{self.port}"

    def getstatus(self):
        self.status = self.query("raw","mainloop_state",onfail=None)
        return self.status

    def getversion(self):
        self.version = self.query("raw","version",onfail=None)
        return self.version

    def wait(self,status=None):
        timer = 0
        retry = self.RETRYTIME
        while (actual:=self.getstatus()) != status and timer < self.TIMEOUT:
            time.sleep(retry)
            timer += retry
            retry *= 2 if retry < 5 else 1
        return actual

    def stop(self):
        """Stop the server normally

        Exceptions:

            GridlabdServerException   Unable to start server and get version
        """
        verbose("stopping server")
        self.query("control","stop",onfail=None)
        if not self.wait() is None:
            exception("unable to stop server")
        elif self.proc:
            self.proc.wait()
            self.proc.stdout.close()
            self.proc.stderr.close()

    def query(self,*args,astype=str,onfail=verbose):
        """Send a query to the server REST API"""
        if self.port is None:
            return None
        url = f"{self.PROTOCOL}://{self.HOST}:{self.port}/{'/'.join(args)}"
        try:
            result = requests.get(url)
            self.status = result.status_code
            verbose(f"query '{url}' --> '{result.text}' (code {result.status_code})")
            return astype(result.text) if result.status_code == 200 else None
        except Exception as err:
            self.status = sys.exc_info()
            verbose(f"query '{url}' --> {self.status[0].__name__}")
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

#
# Unit testing
#
if __name__ == "__main__":

    import unittest
    import tracemalloc
    tracemalloc.start()

    with tempfile.NamedTemporaryFile(suffix=".glm",mode="w+t",delete=True) as fh:

        fh.write(f"""// created by {sys.argv} unittest on {datetime.datetime.now()}
#option warn
clock {{
    timezone "PST+8PDT";
}}
#ifmissing 123.glm
#model get IEEE/123
#endif
#include "123.glm"
#ifmissing "CA-San_Francisco_Intl_Ap.glm" 
#weather get "CA-San_Francisco_Intl_Ap.tmy3"
#endif
#input "CA-San_Francisco_Intl_Ap.tmy3"
#set run_realtime=1
""")
        TMPFILE = fh.name
        class TestServer(unittest.TestCase):

            def test_detached(self):
                """Verify that server can be started detached"""
                fh.seek(0)
                sim = GridlabdServer(fh.name,detached=True)
                self.assertEqual(len(sim.get_objects("class=load")),85)
                sim.stop()

            def test_attached(self):
                """Verify that server can be started attached"""
                fh.seek(0)
                sim = GridlabdServer(fh.name,detached=False)
                now = datetime.datetime.now(GldTimestamp.tz)
                dt = sim.get_global("clock",astype=GldTimestamp)
                self.assertLess(dt-now,datetime.timedelta(seconds=2))
                del sim

            def test_context(self):
                """Verify that server can be started in a context"""
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
                    
        unittest.main()
    if TMPFILE and os.path.exists(TMPFILE):
        warning(f"temporary file {TMPFILE} was not deleted after test completed")
