import os
import numpy as np
import matplotlib.pyplot as plt 
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm


R = 20 	#rows

def listtoarray(matrix):
	arraymatrix = np.zeros((len(matrix), len(matrix[0])))
	for i in range(0,len(matrix)):
		for j in range(0,len(matrix[i])):
			arraymatrix[i][j] = matrix[i][j]

	return arraymatrix

def plot2d(array):
	x = np.arange(0,len(array))
	y = array
	plt.plot(x,y)

def plot3d(matrix, matrix2):
	fig = plt.figure()
	plt.clf()
	ax = fig.gca(projection='3d')
	y = np.arange(0,len(matrix))
	x = np.arange(0,len(matrix[0]))
	z = matrix
	x,y = np.meshgrid(x,y)

	surf = ax.plot_surface(x,y,z,rstride=1,cstride=len(matrix[0]),alpha=0.7, cmap=cm.coolwarm, linewidth=0)
	ax.ticklabel_format(style="sci", axis="z", scilimits=(0,0))


	y2 = np.arange(0,len(matrix2))
	x2 = np.arange(0,len(matrix2[0]))
	x2,y2 = np.meshgrid(x2,y2)
	z2 = matrix2

	surf = ax.plot_wireframe(x2,y2,z2,rstride=1,cstride=len(matrix[0]),color="black", linestyle="-", linewidth=1.5)

	ax.set_ylabel("radius R (deg)")
	ax.set_xlabel("N iterations")
	ax.set_zlabel("<sigma(r,n)>")

	plt.show()

def average_over(start,stop,array):
	avg = 0
	if start == stop:
		return array[start]
	for i in range(start,stop):
	#	print i
		avg += array[i]
	avg /= abs(stop-start)
	return avg

def transpose_matrix(matrix):
	print matrix.shape
	new = np.zeros((len(matrix[0]),len(matrix)))
	for i in range(0,len(matrix)):
		for j in range(0,len(matrix[i])):
			new[j][i] = matrix[i][j]
	print new.shape
	return new

def makeQNR(matrix):
	QNR = np.zeros((len(matrix),len(matrix[0])))
	for i in range(0,len(matrix)): #through 20 r's
		for j in range(0,len(matrix[i])): #through 200 n
			QNR[i][j] = average_over(0,j,matrix[i])
	return QNR



def main():
	plt.clf()
	outdir = "output/"
	#print q_r_n 
	perm1_q = [] #will become a matrix where each entry is the 20 Q(R)'. but only the raw data!!!! need to average to turn into q(n,r)
	perm1_sig = []
	for dir_path, dir_names, file_names in os.walk(outdir):

		for single_file in file_names:
			if single_file.endswith(".txt") and dir_path.startswith("output/out_"):
				
				temppath = dir_path + "/" + single_file
				file_reader = open(temppath,'r')
				temp_list = []

				for line in file_reader:
					temp_list.append(float(line.strip()))

				if single_file.startswith("q_e10_40"):
					perm1_q.append(temp_list)

				if single_file.startswith("sigma_e10_40"):
					perm1_sig.append(temp_list)

		#		print "data files collected = %d" %len(perm1_q)
				#if len(perm1_q) > 0:
		#			print len(perm1_q[len(perm1_q)-1])

	raw1_q = transpose_matrix(listtoarray(perm1_q))
	QNR1_q = makeQNR(raw1_q)
	raw1_sig = transpose_matrix(listtoarray(perm1_sig))
	QNR1_sig = makeQNR(raw1_sig)
	for i in range(0,20):
		plot2d(QNR1_q[i])
		plot2d(QNR1_sig[i])


	plot3d(QNR1_sig, QNR1_q)

				