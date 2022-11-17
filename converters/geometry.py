import math

#determinant of matrix a
def det(a):
	return a[0][0]*a[1][1]*a[2][2] + a[0][1]*a[1][2]*a[2][0] + a[0][2]*a[1][0]*a[2][1] - a[0][2]*a[1][1]*a[2][0] - a[0][1]*a[1][0]*a[2][2] - a[0][0]*a[1][2]*a[2][1]

#unit normal vector of plane defined by points a, b, and c
def unit_normal(a, b, c):
	x = det([[1,a[1],a[2]],
			 [1,b[1],b[2]],
			 [1,c[1],c[2]]])
	y = det([[a[0],1,a[2]],
			 [b[0],1,b[2]],
			 [c[0],1,c[2]]])
	z = det([[a[0],a[1],1],
			 [b[0],b[1],1],
			 [c[0],c[1],1]])
	magnitude = (x**2 + y**2 + z**2)**.5
	return (x/magnitude, y/magnitude, z/magnitude)

#dot product of vectors a and b
def dot(a, b):
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2]

#cross product of vectors a and b
def cross(a, b):
	x = a[1] * b[2] - a[2] * b[1]
	y = a[2] * b[0] - a[0] * b[2]
	z = a[0] * b[1] - a[1] * b[0]
	return (x, y, z)

class Polygon:

	def __init__(self,poly):
		self.poly = poly

	# perimeter of polygon
	def perimeter(self):
		P = 0.0
		pp = self.poly[1:]
		pp.append(self.poly[0])
		for a,b in zip(self.poly,pp):
			P += math.sqrt((a[0]-b[0])**2+(a[1]-b[1])**2+(a[2]-b[2])**2)
		return P

	#area of polygon poly
	def area(self):
		if len(self.poly) < 3: # not a plane - no area
			return 0

		total = [0, 0, 0]
		for i in range(len(self.poly)):
			vi1 = self.poly[i]
			if i is len(self.poly)-1:
				vi2 = self.poly[0]
			else:
				vi2 = self.poly[i+1]
			prod = cross(vi1, vi2)
			total[0] += prod[0]
			total[1] += prod[1]
			total[2] += prod[2]
		result = abs(dot(total, unit_normal(self.poly[0], self.poly[1], self.poly[2])))/2
		# print("Area",poly,"-->",result)
		return result

	def height(self):
		z = [p[2] for p in self.poly]
		return max(z) - min(z)

