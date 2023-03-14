# ampacity calculation, Xiaochu Wang, 3/13/2023
# example from https://citeseerx.ist.psu.edu/document?repid=rep1&type=pdf&doi=e46fd00335c248c5ecbeef370e53374166dc16d4

import control as ct
import numpy as np
import matplotlib.pyplot as plt

pi = np.pi

# temperature
Ta = 25     # ambient temperture, degree C
Tc = 75     # maximum cable core temperature, degree C

# cable data: inches
p_conductor = 0.017     # ohm-meters
Do = 0.6   # inside of annular conductor
Dc = 1.543 # diameter of the segmental conductor
Di = 2.113 # diameter of insulation, outside
T  = 0.285
Ds = 2.373 # diameter of sheath, outside
t  = 0.130 # thickness 
nn = 1     # number of conductors per cable
N  = 6     # number of cables/duct
id_duct = 5 # inside diameter of the duct/conduit
t_duct  = 0.25
od_duct = id_duct+2*t_duct # outside diameter

# calculate conductor resistance
R_dc = 12.9/1.5   # microhms per foot; given 8.6

# thermal resistance: degree-cm/W
rho_insulation = 550  # insulation
rho_jacket = 550
rho_duct = 480        # fiber duct
rho_earth = 120       # earth
rho_concrete = 85     # concrete

# calculate the thermal resistance: Ri, Rsd, Rd
Ri = 0.012*rho_insulation*np.log10(Di/Dc) # thermal ohm-foot
Ap = 4.6
Bp = 0.27 
Rsd = nn*Ap/(Ds+Bp)	# thermal ohm-foot
De = od_duct
Rd = 0.0104*rho_duct*nn*t_duct/(od_duct-t_duct)
print("Ri: " + str(Ri))
print("Rsd: " + str(Rsd))
print("Rd: " + str(Rd))

# calculate the mutual heating factor F
 # write a separate function
L = 37.25 # depth of reference cable below earth's surface, inches
Lb= 37.25 # depth to the center of a duct bank, inches
D21 = 5.5
D31 = 5.5
D41 = 7
D51 = 8.9
D61 = 8.9
D21p = 69
D31p = 80
D41p = 74.83
D51p = 80.31
D61p = 69.35
F = D21p*D31p*D41p*D51p*D61p/D21/D31/D41/D51/D61
print("F: " + str(F))

# calculate duct geometric factor Gb
x = 18.5 # short side of the duct
y = 22.5 # long side of the duct
P = 2*(x+y)
LbP = Lb/P # 0.454
yx = y/x   # 1.22
Gb = 0.82 # according to the provided figure
 # detailed calculation
rb_c1 = x/y/2*(4/pi-x/y)
rb_c2 = np.log10(1+y**2/x**2)
rb_c3 = np.log10(x/2)   
rb = 10**(rb_c1*rb_c2+rb_c3)
print("rb: " + str(rb))
u = Lb/rb         # 3.4
Gb =  np.log10(u+(u**2-1)**0.5) # geometric factor applying to a duct bank 
print("Gb: " + str(Gb)) # now match the table result

# thermal resistance for the whole duct bank including the earth on top
LF = 0.8 # loss factor
Dx = 8.3 # ficitious diameter at which the effect of loss factor commences
De = od_duct # diameter at start of the earth portion of the thermal circuit
Re_1 = np.log10(Dx/De)
Re_2 = LF*np.log10(4*L/Dx*F)
Re_corr = 0.012*(rho_earth-rho_concrete)*nn*N*LF*Gb
Re = 0.012*rho_concrete*nn*(Re_1+Re_2)+Re_corr
print("Re: " + str(Re))

# the total thermal resistance
Rca = Ri + Rsd + Rd + Re
print("Rca: " + str(Rca))

# the ampacity for the ref cable
I = ((Tc-Ta)/R_dc/Rca)**0.5*1000 # in Amp
print("Ampacity (amp): " + str(I))
