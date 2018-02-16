import os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm


Rmax = 20

class myavgclass:
	def __init__(self):
		self.e10_20 = np.zeros(Rmax)
		self.e10_30 = np.zeros(Rmax)
		self.e10_40 = np.zeros(Rmax)
		self.e20_30 = np.zeros(Rmax)
		self.e20_40 = np.zeros(Rmax)
		self.e30_40 = np.zeros(Rmax)
		self.e10_20_saveall = np.zeros((50,Rmax))
		self.e10_30_saveall = np.zeros((50,Rmax))
		self.e10_40_saveall = np.zeros((50,Rmax))
		self.e20_30_saveall = np.zeros((50,Rmax))
		self.e20_40_saveall = np.zeros((50,Rmax))
		self.e30_40_saveall = np.zeros((50,Rmax))

def normalize(arr, n):
	for i in range (0, len(arr)):
		arr[i] /= n
	return arr

def avg_it(arr, start, stop):
	avg = 0
	for i in range(start, stop):
	#	print arr[i]
		avg += arr[i]
	avg /= (abs(stop-start))
	return avg

def write_results(avg_q, avg_sigma):
	out_avg = "averaged_output/"

	file_q_e10_20 = open(out_avg + "q_e10_20.txt", "w")
	for item in avg_q.e10_20:
		file_q_e10_20.write("%s\n" %item)
	file_q_e10_30 = open(out_avg + "q_e10_30.txt", "w")
	for item in avg_q.e10_30:
		file_q_e10_30.write("%s\n" %item)
	file_q_e10_40 = open(out_avg + "q_e10_40.txt", "w")
	for item in avg_q.e10_40:
		file_q_e10_40.write("%s\n" %item)
	file_q_e20_30 = open(out_avg + "q_e20_30.txt", "w")
	for item in avg_q.e20_30:
		file_q_e20_30.write("%s\n" %item)
	file_q_e20_40 = open(out_avg + "q_e20_40.txt", "w")
	for item in avg_q.e20_40:
		file_q_e20_40.write("%s\n" %item)
	file_q_e30_40 = open(out_avg + "q_e30_40.txt", "w")
	for item in avg_q.e30_40:
		file_q_e30_40.write("%s\n" %item)

	file_sigma_e10_20 = open(out_avg + "sigma_e10_20.txt", "w")
	for item in avg_sigma.e10_20:
		file_sigma_e10_20.write("%s\n" %item)
	file_sigma_e10_30 = open(out_avg + "sigma_e10_30.txt", "w")
	for item in avg_sigma.e10_30:
		file_sigma_e10_30.write("%s\n" %item)
	file_sigma_e10_40 = open(out_avg + "sigma_e10_40.txt", "w")
	for item in avg_sigma.e10_40:
		file_sigma_e10_40.write("%s\n" %item)
	file_sigma_e20_30 = open(out_avg + "sigma_e20_30.txt", "w")
	for item in avg_sigma.e20_30:
		file_sigma_e20_30.write("%s\n" %item)
	file_sigma_e20_40 = open(out_avg + "sigma_e20_40.txt", "w")
	for item in avg_sigma.e20_40:
		file_sigma_e20_40.write("%s\n" %item)
	file_sigma_e30_40 = open(out_avg + "sigma_e30_40.txt", "w")
	for item in avg_sigma.e30_40:
		file_sigma_e30_40.write("%s\n" %item)

def make_brazil(arrq_matrix, arrsig_matrix, N):
	N=50
	n = np.arange(1,N+1)
	zref = np.zeros(N)
	which = 5

	#plt.plot(n, zref, 'k--')

	
	for j in range(0,Rmax):

		arrq = []
		arrsig = []
		for i in range(0,N):
			arrq.append(arrq_matrix[i][j])
			arrsig.append(arrsig_matrix[i][j])
		sig_n = [arrsig[0]]
		q_n = [arrq[0]]
	#running_sig = arrsig[0]
		for i in range(1,N):
			#print i
			sig_n.append(avg_it(arrsig, 0, i))
			q_n.append(avg_it(arrq, 0, i))
			
		#plt.plot(n, sig_n)
		#plt.plot(n, q_n)
		
	fig = plt.figure(1)

	ax = fig.gca(projection='3d')
	#data
	x = np.arange(0,Rmax)
	#print x
	y = np.arange(0,50)
	#print y
	xx, yy = np.meshgrid(x,y)
	z = arrq_matrix
	print type(z)
	#xxx = np.reshape(x,z.shape)
	#yyy = np.reshape(y,z.shape)

	#print arrq_matrix
	#arrq_matrix[x][y]
	

	surf = ax.plot_surface(xx, yy, z,rstride=1,cstride=1,cmap=cm.coolwarm)

	ax.set_xlabel("radius R (deg)")
	ax.set_ylabel("n iterations")
	ax.set_zlabel("<Q(R,n)>")

	plt.show()



def main():
	outdir = "output/"
	N = -1
	avg_q = myavgclass()
	avg_sigma = myavgclass()

	for dir_path, dir_names, file_names in os.walk(outdir):		# walk() returns three values always. 
		#print "dir_path = \t%s"  %dir_path 		#now in a directory, ex. out_0001
		for single_file in file_names:			#loop over files in directory
			if single_file.endswith('.txt'):		#make sure we extract from text files only
			#	print "single_file = \t%s"  %single_file	
				temppath = dir_path + "/" + single_file
				file_reader = open(temppath, 'r')	
				temparr = []						#save data from each text file
				for line in file_reader:
					val = float(line.strip())
					temparr.append(val)

				if single_file.startswith("q_e10_20"):
					for i in range(0, Rmax):
							avg_q.e10_20[i] += temparr[i]
					#avg_q.e10_20_saveall.append(temparr[Rmax-1])
					avg_q.e10_20_saveall[i] = temparr
					#print avg_q.e10_20_saveall[0]
					#print temparr[Rmax-1]
				elif single_file.startswith("q_e10_30"):
					for i in range(0, Rmax):
							avg_q.e10_30[i] += temparr[i]
					#avg_q.e10_30_saveall.append(temparr[Rmax-1])
					avg_q.e10_30_saveall[i] = temparr
				elif single_file.startswith("q_e10_40"):
					for i in range(0, Rmax):
							avg_q.e10_40[i] += temparr[i]
					#avg_q.e10_40_saveall.append(temparr[Rmax-1])
					avg_q.e10_40_saveall[i] = temparr
				elif single_file.startswith("q_e20_30"):
					for i in range(0, Rmax):
							avg_q.e20_30[i] += temparr[i]
					#avg_q.e20_30_saveall.append(temparr[Rmax-1])
					avg_q.e20_30_saveall[i] = temparr
				elif single_file.startswith("q_e20_40"):
					for i in range(0, Rmax):
							avg_q.e20_40[i] += temparr[i]
					#avg_q.e20_40_saveall.append(temparr[Rmax-1])
					avg_q.e20_40_saveall[i] = temparr
				elif single_file.startswith("q_e30_40"):
					for i in range(0, Rmax):
							avg_q.e30_40[i] += temparr[i]
					#avg_q.e30_40_saveall.append(temparr[Rmax-1])
					avg_q.e30_40_saveall[i] = temparr
			########################################################
				elif single_file.startswith("sigma_e10_20"):
					for i in range(0, Rmax):
							avg_sigma.e10_20[i] += temparr[i]
					#avg_sigma.e10_20_saveall.append(temparr[Rmax-1])
					avg_sigma.e10_20_saveall[i] = temparr
				elif single_file.startswith("sigma_e10_30"):
					for i in range(0, Rmax):
							avg_sigma.e10_30[i] += temparr[i]
					#avg_sigma.e10_30_saveall.append(temparr[Rmax-1])
					avg_sigma.e10_30_saveall[i] = temparr
				elif single_file.startswith("sigma_e10_40"):
					for i in range(0, Rmax):
							avg_sigma.e10_40[i] += temparr[i]
					#avg_sigma.e10_40_saveall.append(temparr[Rmax-1])
					avg_sigma.e10_40_saveall[i] = temparr
				elif single_file.startswith("sigma_e20_30"):
					for i in range(0, Rmax):
							avg_sigma.e20_30[i] += temparr[i]
					#avg_sigma.e20_30_saveall.append(temparr[Rmax-1])
					avg_sigma.e20_30_saveall[i] = temparr
				elif single_file.startswith("sigma_e20_40"):
					for i in range(0, Rmax):
							avg_sigma.e20_40[i] += temparr[i]
					#avg_sigma.e20_40_saveall.append(temparr[Rmax-1])
					avg_sigma.e20_40_saveall[i] = temparr
				elif single_file.startswith("sigma_e30_40"):
					for i in range(0, Rmax):
							avg_sigma.e30_40[i] += temparr[i]
					#avg_sigma.e30_40_saveall.append(temparr[Rmax-1])	
					avg_sigma.e30_40_saveall[i] = temparr
		N +=1 
		#print N
		#end for loop
	avg_q.e10_20 = normalize(avg_q.e10_20, N)
	avg_q.e10_30 = normalize(avg_q.e10_30, N)
	avg_q.e10_40 = normalize(avg_q.e10_40, N)
	avg_q.e20_30 = normalize(avg_q.e20_30, N)
	avg_q.e20_40 = normalize(avg_q.e20_40, N)
	avg_q.e30_40 = normalize(avg_q.e30_40, N)

	avg_sigma.e10_20 = normalize(avg_sigma.e10_20, N)
	avg_sigma.e10_30 = normalize(avg_sigma.e10_30, N)
	avg_sigma.e10_40 = normalize(avg_sigma.e10_40, N)
	avg_sigma.e20_30 = normalize(avg_sigma.e20_30, N)
	avg_sigma.e20_40 = normalize(avg_sigma.e20_40, N)
	avg_sigma.e30_40 = normalize(avg_sigma.e30_40, N)


	write_results(avg_q, avg_sigma)

	
	make_brazil(avg_q.e10_20_saveall, avg_sigma.e10_20_saveall, N)
	plot1, = plt.plot([], [], color='#8b0000', linewidth=2, label="1 sig")
	#plot2, = plt.plot([], [], ".", color="#6897bb", linewidth=10, label="q")

	plt.title("{10,20} GeV")
	
	plt.legend(loc="upper left")

	
	plt.rcParams.update({'figure.autolayout': True})
	plt.show()

