"""Ductbank calculations"""

from math import *

# heat transfer coefficients
U = { 			# W/K.m (conductivity)
    "PVC" 		: 0.19,
    "CONCRETE" 	: 0.92,
    "WETSOIL" 	: 1.00,
    "SOIL" 		: 0.80,
    "DRYSOIL" 	: 0.60,
    "RUBBER" 	: 0.10,
    "PE" 		: 0.46,
    "AIR" 		: 0.20, # includes convective and radiative effects
}

default_ground_temperature = 25.0

class DuctBank:

	def __init__(self,**kwargs):

		N = self.getarg(kwargs,"number_of_cables")
		M = self.getarg(kwargs,"number_of_ducts",1)
		Q = self.getarg(kwargs,"total_cable_losses")
		A0 = self.getarg(kwargs,"air_section_area")
		TG = self.getarg(kwargs,"ground_temperature",default_ground_temperature)
		DI = self.getarg(kwargs,"cable_insulation_thickness")
		DS = self.getarg(kwargs,"cable_outer_diameter")
		DC = self.getarg(kwargs,"duct_core_thickness")
		DF = self.getarg(kwargs,"duct_fill_thickness")

		IA = N * DS * pi
		self.setattr("insulation_area",IA)

		UI = U[self.getarg(kwargs,"insulation_type")] / DI * IA
		self.setattr("insulation_U",UI)

		AC = N * (DS/4)**2 * pi
		self.setattr("cable_section_area",AC)

		A = ( A0 - AC ) / M
		self.setattr("air_gap_area",A)

		if A < 0:
			raise ValueError("duct is too small")

		D = sqrt(A) / 2
		self.setattr("mean_air_gap",D)

		AD = 4 * sqrt(A)
		self.setattr("effective_air_area",AD)

		UA = U["AIR"]/D*AD
		self.setattr("air_U",UA)

		UC = U[self.getarg(kwargs,"core_material")] / DC
		UF = U[self.getarg(kwargs,"fill_material")] / DF
		UD = AD / (1/UC+1/UF)
		self.setattr("duct_U",UD)

		TD = TG + Q/UD
		self.setattr("duct_T",TD)

		TS = TD + Q/UA
		self.setattr("shield_T",TS)

		TC = TS + Q/UI
		self.setattr("cable_T",TC)

	def getarg(self,args,name,default=None,required=True):
		value = args[name] if name in args else default
		if value == None and required == True:
			raise ValueError(f"{name} is required")
		setattr(self,name,value)
		print(f"{name:20s} = {value}")
		return value

	def setattr(self,name,value):
		old = getattr(self,name) if hasattr(self,name) else None
		setattr(self,name,value)
		print(f"{name:20s} = {value:.3g}")
		return old

if __name__ == "__main__":

	import unittest

	class TestDuctBank(unittest.TestCase):

		def test_0(self):
			specs = dict(
				number_of_cables = 6,
				total_cable_losses = 6,
				air_section_area = 0.01,
				cable_insulation_thickness = 0.01,
				cable_outer_diameter = 0.025,
				duct_core_thickness = 0.40,
				duct_fill_thickness = 0.60,
				insulation_type = 'RUBBER',
				core_material = 'CONCRETE',
				fill_material = 'SOIL',
				)
			duct = DuctBank(**specs)
			self.assertEqual(round(duct.cable_T,1),48.5)

	unittest.main()
