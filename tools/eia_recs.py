"""EIA RECS data accessor

This package provides access to the EIA RECS data
"""
import os, sys
import openpyxl
import requests
import datetime
import pandas
import uuid

microdata_url = "https://www.eia.gov/consumption/residential/data/2015/csv/recs2015_public_v4.csv"
hc_baseurl = "https://www.eia.gov/consumption/residential/data/2015/hc/"

class HousingCharacteristics:
    """Housing characteristics class implementation"""
    
    table_sources = {
        "2015" : {
            "hc1.1" : "Fuel use and end-uses by housing unit type",
        }
    }

    table_structure = {
        "hc1.1" : {
            "units" : 1e6,
            "columns" : {
                "total" : "B",
                "unit-type" : {
                    "single-family-detached" : "C",
                    "single-family-attached" : "D",
                    "apartment-small" : "E",
                    "apartment-large" : "F",
                    "mobile-home" : "G",
                },
            },
            "rows" : {
                "total" : 6,
                "fuel-used" : {
                    "electricity" : 8,
                    "natural-gas" : 9,
                    "propane" : 10,
                    "wood" : 11,
                    "fuel-oil" : 12,
                },
                "electric-end-use" : {
                    "space-heating" : {
                        "total" : 14,
                        "main" : 15,
                        "secondary" : 16,
                        },
                    "air-conditioning" : 17,
                    "water-heating" : 18,
                    "cooking" : 19,
                },
                "natural-gas-end-use" : {
                    "space-heating" : {
                        "total" : 21,
                        "main" : 22,
                        "secondary" : 23,
                        },
                    "water-heating" : 24,
                    "cooking" : 25,
                    "outdoor-grilling" : 26,
                },
                "natural-gas-end-use" : {
                    "space-heating" : {
                        "total" : 28,
                        "main" : 29,
                        "secondary" : 30,
                        },
                    "water-heating" : 31,
                    "cooking" : 32,
                    "outdoor-grilling" : 33,
                },
                "wood-end-use" : {
                    "space-heating" : {
                        "total" : 35,
                        "main" : 36,
                        "secondary" : 37,
                        },
                    "water-heating" : 38,
                },
                "fuel-oil-end-use" : {
                    "space-heating" : {
                        "total" : 40,
                        "main" : 41,
                        "secondary" : 42,
                        },
                    "water-heating" : 43,
                },
            },
        },
    }

    def __init__(self,table):
        """Construct a housing characteristics from a RECS table

        PARAMETERS

            table (str) - The RECS table number (e.g., "1.1")
        """
        self.name = "hc"+table
        xlsname = f"{self.name}.xlsx"
        if not os.path.exists(xlsname):
            req = requests.get(f"{hc_baseurl}{self.name}.xlsx")
            if req.status_code != 200:
                return None
            with open(xlsname,"wb") as f:
                f.write(req.content)
        self.book = openpyxl.load_workbook(xlsname)
        self.released_on = datetime.datetime.strptime(self.book['data']['A1'].value.split("\n")[0].split(":")[1].strip(),"%B %Y")
        self.revised_on = datetime.datetime.strptime(self.book['data']['A1'].value.split("\n")[1].split(":")[1].strip(),"%B %Y")

    def find(self,sheet,row=None,column=None):
        """Obtain a value in a housing characteristics table

        PARAMETERS

            sheet (str) - specify the sheet, e.g., "data" or "rse"

            row (str or list of str) - specify the column to find

            column (int or list of str) - specify the row to find

        RETURNS

            (float) - the value if found

            (row data, column data) - the row and column specifications if the value is not found
        """
        book = self.book[sheet]
        rows = self.table_structure[self.name]["rows"]
        columns = self.table_structure[self.name]["columns"]
        while type(row) is list:
            rows = rows[row[0]]
            if len(row) > 1:
                row = row[1:]
            else:
                row = row[0]
        while type(column) is list:
            # print("Column:",column)
            columns = columns[column[0]]
            if len(column) > 1:
                column = column[1:]
            else:
                column = column[0]
        try:
            return float(book[f"{columns}{rows}"].value * self.table_structure[self.name]["units"])
        except:
            if type(rows) is dict:
                rows = list(rows.keys())
            if type(columns) is dict:
                columns = list(columns.keys())
            return [rows,columns]

class Microdata(pandas.DataFrame):
    """RECS Microdata class implementation"""

    regions = {
        "northeast": {
            "id" : 1,
            "divisions" : {
                "new-england" : {
                    "id" : 1,
                    "states" : {
                        "CT" : 9,
                        "ME" : 23,
                        "MA" : 25,
                        "NH" : 33,
                        "RI" : 44,
                        "VT" : 50,
                    },
                },
                "middle-atlantic" : {
                    "id" : 2,
                    "states" : {
                        "NJ" : 34,
                        "NY" : 36,
                        "PA" : 42,
                    },                  
                },
            },
        },
        "midwest" : {
            "id" : 2,
            "divisions" : {
                "east-north-central" : {
                    "id" : 3,
                    "states" : {
                        "IN" : 18,
                        "IL" : 17,
                        "MI" : 26,
                        "OH" : 39,
                        "WI" : 55,
                    },
                },
                "west-north-central" : {
                    "id" : 4,
                    "states" : {
                        "IA" : 19,
                        "KS" : 20,
                        "MN" : 27,
                        "MO" : 29,
                        "NE" : 31,
                        "ND" : 38,
                        "SD" : 46,
                    },
                },
            },
        },
        "south" : {
            "id" : 3,
            "divisions" : {
                "south-atlantic" : {
                    "id" : 5,
                    "states" : {
                        "DE" : 10,
                        "DC" : 11,
                        "FL" : 12,
                        "GA" : 13,
                        "MD" : 24,
                        "NC" : 37,
                        "SC" : 45,
                        "VA" : 51,
                        "WV" : 54,
                    },
                },
                "east-south-central" : {
                    "id" : 6,
                    "states" : {
                        "AL" : 1,
                        "KY" : 21,
                        "MS" : 28,
                        "TN" : 47,
                    },
                },
                "west-south-centrl" : {
                    "id" : 7,
                    "states" : {
                        "AR" : 5,
                        "LA" : 22,
                        "OK" : 40,
                        "TX" : 48,
                    },
                },
            },
        },
        "west" : {
            "id" : 4,
            "divisions" : {
                "mountain" : {
                    "id" : 8,
                    "states" : {
                        "AZ" : 4,
                        "CO" : 8,
                        "ID" : 16,
                        "NM" : 35,
                        "MT" : 30,
                        "UT" : 49,
                        "NV" : 32,
                        "WY" : 56,
                    },
                },
                "pacific" : {
                    "id" : 9,
                    "states" : {
                        "AK" : 2,
                        "CA" : 6,
                        "HI" : 15,
                        "OR" : 41,
                        "WA" : 53,
                    },
                },
            },
        },
    }
    def __init__(self):
        """Data frame constructor"""
        csvname = f".hc_{str(uuid.uuid4().hex)}.csv"
        if not os.path.exists(csvname):
            req = requests.get(microdata_url)
            if req.status_code != 200:
                return None
            with open(csvname,"wb") as f:
                f.write(req.content)
        pandas.DataFrame.__init__(self,pandas.read_csv(csvname))
        os.unlink(csvname)
    
    @classmethod
    def get_division(cls,statecodes=[]):
        """Get region and division names for a state or states"""
        result = {}
        for region, divisions in cls.regions.items():
            for division, states in divisions["divisions"].items():
                for state, fips in states["states"].items():
                    if not statecodes or state == statecodes or state in statecodes:
                        result[fips] = dict(region=region,division=division,state=state)
        if type(statecodes) == str:
            return result[list(result.keys())[0]]
        else:
            return result

    @classmethod
    def get_codes(cls,region,division=None,state=None):
        """Get census region, division, and fips codes"""
        if not division:
            return {"region" : cls.regions[region]["id"]}
        elif not state:
            return {"region" : cls.regions[region]["id"], 
                "division" : cls.regions[region]["divisions"][division]["id"]}
        else:
            return {"region" : cls.regions[region]["id"], 
                "division" : cls.regions[region]["divisions"][division]["id"], 
                "fips" : cls.regions[region]["divisions"][division]["states"][state]}

import unittest

class _test(unittest.TestCase):

    def test_hc1_1_0(self):
        hc = HousingCharacteristics(table="1.1")
        self.assertEqual(hc.find('data'),[['total', 'fuel-used', 'electric-end-use', 'natural-gas-end-use', 'wood-end-use', 'fuel-oil-end-use'], ['total', 'unit-type']])

    def test_hc1_1_1(self):
        hc = HousingCharacteristics(table="1.1")
        self.assertEqual(hc.find('data',['total'],['total']),118.2e6)

    def test_hc1_1_2(self):
        hc = HousingCharacteristics(table="1.1")
        self.assertEqual(hc.find('data',['total'],['unit-type','single-family-detached']),73.9e6)

    def test_hc1_1_3(self):
        hc = HousingCharacteristics(table="1.1")
        self.assertEqual(hc.find('data',['electric-end-use','space-heating'],['total']),[['total', 'main', 'secondary'], 'B'])

    def test_hc1_1_4(self):
        hc = HousingCharacteristics(table="1.1")
        self.assertEqual(hc.find('data',['fuel-used','natural-gas'],['total']),68.6e6)

    def test_microdata(self):
        md = Microdata()
        self.assertEqual(md["LPXBTU"][0],91.33)

    def test_microdata_fips(self):
        fips = Microdata.get_division("NY")
        self.assertEqual(fips["region"],"northeast") 
        self.assertEqual(fips["division"],"middle-atlantic") 
        self.assertEqual(Microdata.regions[fips["region"]]["id"],1) 
        self.assertEqual(Microdata.regions[fips["region"]]["divisions"][fips["division"]]["id"],2) 
        self.assertEqual(Microdata.regions[fips["region"]]["divisions"][fips["division"]]["states"]["NY"],36) 

    def test_microdata_codes(self):
        self.assertEqual(Microdata.get_codes("northeast")["region"],1)
        self.assertEqual(Microdata.get_codes("northeast","middle-atlantic")["division"],2)
        self.assertEqual(Microdata.get_codes("northeast","middle-atlantic","NY")["fips"],36)

if __name__ == "__main__":
    unittest.main()
