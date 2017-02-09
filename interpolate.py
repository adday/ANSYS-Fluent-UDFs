import numpy as np	

def interpolate(x,y):
z_numer = 0
z_denom = 0
for i in range(len(X)):
	if X[i] == x and Y[i] == y:
		return Z[i]
	else:
		z_numer += Z[i] / ((X[i] - x)**2 + (Y[i]-y)**2)**4
		z_denom += 1 / ((X[i] - x)**2 + (Y[i]-y)**2)**4
return z_numer/z_denom
			
		
			
y_new = -1
X_Y_new = []
min = sum = 0
for i in range(len(np.unique(data[:,0]))):
	for j in range(len(X)):
		if data[j,0] == i:
			if abs(data[j,3] - 0.5) < min	
				min = abs(data[j,3] - 0.5)
				y_new = data[j]
	X_Y_new.append(y_new)
	
			
			
			
			
			
			
			