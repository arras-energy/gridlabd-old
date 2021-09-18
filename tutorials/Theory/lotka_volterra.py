import matplotlib.pyplot as plt
from numpy import arange, repeat
from math import log

a = 0.5
b = 1.5
c = 1
d = 1
dt = 0.001

x0 = d/c
y0 = a/b

X0 = arange(0.1,x0,0.1)
Y0 = repeat(round(y0,1),len(X0))

plt.figure(figsize=(15,5))
plt.subplot(111).set_aspect('equal')
plt.plot(x0,y0,'xb',label='Fixed point')
plt.plot(X0,Y0,'vk',label='Initial points')
plt.grid()

for x,y in zip(X0,Y0):
	X = [x]
	Y = [y]
	while len(X)<1/dt or (X[0]-X[-1])**2 + (Y[0]-Y[-1])**2 > dt/10:
		dx = a*x - b*x*y
		dy = d*x*y - c*y
		x += dx*dt
		y += dy*dt
		X.append(x)
		Y.append(y)
	plt.plot(X,Y,'-',label=f"$C={d*x-c*log(x)+b*y-a*log(y):.2f}$")

plt.xlabel('Prey\n(x1000)')
plt.ylabel('Predators\n(x1000)')
plt.legend(loc="center right",bbox_to_anchor=(1.25,0.5))
plt.title(f"Lotka-Volterra Model\n($a={a:.1f}$, $b={b:.1f}$, $c={c:.1f}$, $d={d:.1f}$)")

plt.show()
