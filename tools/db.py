"""GridLAB-D database access library"""

import sys, os
import sqlite3

VERBOSE = False
QUIET = False
SILENT = False
DEBUG = False
FLUSH = False
STDERR = sys.stderr
STDOUT = sys.stdout

E_OK = 0
E_INVALID = 1

def error(msg,code):
    if not SILENT:
        print(f"ERROR [db]: {msg}",flush=FLUSH,file=STDERR)
    if type(code) is int:
        exit(code)

def warning(msg):
    if not QUIET:
        print(f"WARNING [db]: {msg}",flush=FLUSH,file=STDERR)

def verbose(msg):
    if VERBOSE:
        print(f"VERBOSE [db]: {msg}",flush=FLUSH,file=STDERR)

class GridlabdDbException(Exception):
    pass

def exception(msg):
    raise GridlabdDbException(msg)

class GridlabdDb:
    """GridLAB-D database accessor"""

    def __init__(self,name=""):
        if name and not os.path.exists(name):
            os.system(f"gridlabd -o {name}")
        self.name = name

    def __enter__(self):
        verbose(f"USE {self.name};")
        self.db = sqlite3.connect(self.name)
        self._begin()
        return self

    def __exit__(self, e_type, e_value, e_trace):
        self.commit()
        self.db.close()
        verbose(f"% closing {self.name}")

    def __getitem__(self,item):
        spec = item.split(':')
        cursor = self.query(f"select value from gridlabd_objects where name = '{':'.join(spec[:-1])}' and property = '{spec[-1]}';")
        result = cursor.fetchone()
        return result[0] if result else None

    def __setitem__(self,item,value):
        spec = item.split(':')
        return self.query(f"update gridlabd_objects set value = '{value}' where name = '{':'.join(spec[:-1])}' and property = '{spec[-1]}';")

    def query(self,sql):
        """Execute a SQL query directly on the database"""
        verbose(sql)
        return self.db.execute(sql)

    def load(self,name='gridlabd.json'):
        """Load a database from a JSON file"""
        raise NotImplementedException()

    def save(self,name='gridlabd.json'):
        """Save a database to GLM file"""
        raise NotImplementedException()


    def _begin(self):
        self.query("BEGIN TRANSACTION;")

    def commit(self):
        """Save the pending changes to the database"""
        self.query("END TRANSACTION;")
        self._begin()

    def rollback(self):
        """Forget the pending changes to the database"""
        self.query("ROLLBACK TRANSACTION;")
        self._begin()

    def module(self,name):
        pass

if __name__ == "__main__":

    import unittest

    with GridlabdDb(name="autotest/test_db.db") as db:
        db['scheme_1:armed'] = 'NONE'

    class TestGridlabdDb (unittest.TestCase):

        def test_default(self):

            with GridlabdDb() as db:
                self.assertEqual(db.name,"")

        def test_get(self):
            with GridlabdDb(name="autotest/test_db.db") as db:
                self.assertEqual(db['scheme_1:armed'],'NONE')

        def test_get_fail(self):
            with GridlabdDb(name="autotest/test_db.db") as db:
                self.assertEqual(db['scheme_1:nosuch'],None)

        def test_set_nosave(self):
            with GridlabdDb(name="autotest/test_db.db") as db:
                db['scheme_1:armed'] = 'TRUE'
                self.assertEqual(db['scheme_1:armed'],'TRUE')
                db.rollback()
                self.assertEqual(db['scheme_1:armed'],'NONE')

        def test_set_save(self):
            with GridlabdDb(name="autotest/test_db.db") as db:
                db['scheme_1:armed'] = 'TRUE'
                self.assertEqual(db['scheme_1:armed'],'TRUE')
                db.commit()
                self.assertEqual(db['scheme_1:armed'],'TRUE')

        def test_create(self):
            os.remove("new.db")
            with GridlabdDb("new.db") as db:
                self.assertEqual(db.name,"new.db")

    unittest.main()