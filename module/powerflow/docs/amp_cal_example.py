# ampacity calculation, Xiaochu Wang, 4/12/2023
# example from https://citeseerx.ist.psu.edu/document?repid=rep1&type=pdf&doi=e46fd00335c248c5ecbeef370e53374166dc16d4

import numpy as np

# input1: temperature
Ta = 25     # ambient temperture, degree C
Tc = 75     # maximum cable core temperature, degree C

# input2: conductor resistance
R_dc = 8.6   # microhms per foot; at 25 degree C

# input3: cable data: inches
Do = 0.6   # inside of annular conductor
Dc = 1.543 # diameter of the segmental conductor
Di = 2.113 # diameter of insulation, outside
T  = 0.285
Ds = 2.373 # diameter of sheath, outside
t  = 0.130 # thickness 

# input4: duct bank data
nn = 1     # number of conductors per cable [abc]
N  = 6     # number of cables/duct
id_duct = 5 # inside diameter of the duct/conduit
t_duct  = 0.25
od_duct = id_duct+2*t_duct # outside diameter

# input5: duct bank layout
# general inputs - example from the paper
row = 3
col = 2
# (1) example from the paper
L = 43.5 # depth of reference cable below earth's surface, inches
d_vert = 9
d_hori = 9
x = 18      # short side of the duct
y = 27      # long side of the duct
Lb= 43.5    # depth to the center of a duct bank, inches
# (2) provided example
# L = 37.25 # depth of reference cable below earth's surface, inches
# d_vert = 5.5
# d_hori = 7
# x = 18.5    # short side of the duct
# y = 22.5    # long side of the duct
# Lb= 37.25 # depth to the center of a duct bank, inches

# input6: thermal resistance: degree-cm/W
rho_insulation = 550  # insulation
rho_jacket = 550
rho_duct = 480        # fiber duct
rho_earth = 120       # earth
rho_concrete = 85     # concrete

# input 7: concrete/earth thermal resistance
LF = 0.8 # loss factor
Dx = 8.3 # ficitious diameter at which the effect of loss factor commences
De = od_duct # diameter at start of the earth portion of the thermal circuit

# cal1: calculate the thermal resistance: Ri, Rsd, Rd
Ri = 0.012*rho_insulation*np.log10(Di/Dc) # thermal ohm-foot
Ap = 4.6    # obtain from the table [abc]
Bp = 0.27 
Rsd = nn*Ap/(Ds+Bp) # thermal ohm-foot
De = od_duct
Rd = 0.0104*rho_duct*nn*t_duct/(od_duct-t_duct)
print("Ri: " + str(Ri))
print("Rsd: " + str(Rsd))
print("Rd: " + str(Rd))


# cal2: function in calculating the mutual heating factor F
def cal_mut_heat(row, col, L, d_vert, d_hori): 
    N = row*col     # total # of duct banks
    d_diag = (d_vert**2 + d_hori**2)**0.5
    d_vec = np.array([1, d_vert, d_vert, d_hori, d_diag, d_diag])
    d_diag_short = (d_hori**2 + (2*L-d_vert)**2)**0.5
    d_diag_mid   = (d_hori**2 + (2*L)**2)**0.5
    d_diag_long  = (d_hori**2 + (2*L+d_vert)**2)**0.5
    D_vec = np.array([1, 2*L-d_vert, 2*L+d_vert, d_diag_short, d_diag_mid, d_diag_long])
    product1 = np.prod(d_vec)
    product2 = np.prod(D_vec)
    F = round(product2/product1)

    return F

F = cal_mut_heat(row, col, L, d_vert, d_hori)
print("Mutual heating factor F:" + str(F))

# cal3: function in calculating the geometric factor Gb
def cal_geo_factor(x, y, Lb):
    rb_c1 = x/y/2*(4/np.pi-x/y)
    rb_c2 = np.log10(1+y**2/x**2)
    rb_c3 = np.log10(x/2)   
    rb = 10**(rb_c1*rb_c2+rb_c3)
    u = Lb/rb         # 3.4
    Gb =  np.log10(u+(u**2-1)**0.5) # geometric factor applying to a duct bank 
    Gb = round(Gb, 2)

    return Gb

Gb = cal_geo_factor(x, y, Lb)
print("Duct bank geometric factor Gb: " + str(Gb)) # now match the table result

# cal4: thermal resistance for the whole duct bank including the earth on top
Re_1 = np.log10(Dx/De)
Re_2 = LF*np.log10(4*L/Dx*F)
Re_corr = 0.012*(rho_earth-rho_concrete)*nn*N*LF*Gb
Re = 0.012*rho_concrete*nn*(Re_1+Re_2)+Re_corr
print("Re: " + str(Re))


# the total thermal resistance
Rca = Ri + Rsd + Rd + Re
print("Rca: " + str(Rca))

# cal5: the ampacity for the ref cable
I = ((Tc-Ta)/R_dc/Rca)**0.5*1000 # in Amp
I = round(I)
print("Ampacity (amp): " + str(I))

