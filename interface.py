import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def get_data():
	data = np.genfromtxt('interface.csv',delimiter=',')

	idx = []
	for i in range(len(data)):
		if data[i,3] < .3 or data[i,3] > .7:
			idx.append(i)
	surface = np.delete(data,idx,axis=0)
	fig = plt.figure()

	X = surface[:,0]
	Y = surface[:,2]
	Z = surface[:,1]
	return X,Y,Z,data
	
def interpolate(x,y,X,Y,Z):
	z_numer = 0
	z_denom = 0
	for i in range(len(X)):
		if X[i] == x and Y[i] == y:
			return Z[i]
		else:
			z_numer += Z[i] / ((X[i] - x)**2 + (Y[i]-y)**2)**2
			z_denom += 1 / ((X[i] - x)**2 + (Y[i]-y)**2)**2
	return z_numer/z_denom
	
def grid(X,Y,Z,num_pts=50):
	X_mesh = np.linspace(0.005,0.02,num_pts)
	Y_mesh = np.linspace(0.01,0.06,num_pts)
	
	X_coords, Y_coords, Z_coords = [],[],[]
	for i in range(num_pts):
		for j in range(num_pts):
			X_coords.append(X_mesh[i])
			Y_coords.append(Y_mesh[j])
			Z_coords.append(interpolate(X_mesh[i],Y_mesh[j],X,Y,Z))
	return X_coords, Y_coords, Z_coords

def normal_vectors(X,Y,Z,delta):
	U, V, W = [],[],[]
	for i in range(len(X)):
		x0, y0, z0 = X[i], Y[i], Z[i]
		x1, y1 = x0+delta, y0
		z1 = interpolate(x1,y1,X,Y,Z)
		x2, y2 = x0, y0+delta
		z2 = interpolate(x2,y2,X,Y,Z)
		u = (z2-z0)*(y1-y0)-(z1-z0)*delta
		v = (x2-x0)*(z1-z0)-(z2-z0)*delta
		w = delta**2-(x2-x0)*(y1-y0)
		norm = np.sqrt(u**2+v**2+w**2)
		U.append(u/norm)
		V.append(v/norm)
		W.append(w/norm)
	return U,V,W

	
X,Y,Z,_ = get_data()
X,Y,Z = grid(X,Y,Z)	
U,V,W = normal_vectors(X,Y,Z,0.00025)


fig = plt.figure()
ax = fig.gca(projection='3d')
plt.hold(True)

ax.set_xlim([0,.05])
ax.set_ylim([0,0.07])
ax.set_zlim([0.05,.1])

ax.plot_trisurf(X, Y, Z, cmap='Blues', linewidth=0)

ax.quiver(X, Y, Z, U, V, W, length=0.0004, colors='r', arrow_length_ratio = 0.2, pivot = 'tail')


ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

plt.show()

