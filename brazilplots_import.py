import os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick

Rmax = 20

class myavgclass:
	def __init__(self):
		self.e10_20 = np.zeros(Rmax)
		self.e10_30 = np.zeros(Rmax)
		self.e10_40 = np.zeros(Rmax)
		self.e20_30 = np.zeros(Rmax)
		self.e20_40 = np.zeros(Rmax)
		self.e30_40 = np.zeros(Rmax)

def normalize(arr, n):
	for i in range (0, len(arr)):
		arr[i] /= n
	return arr

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

def make_brazil(q, sig, realq, realsig):
	r = np.arange(1,Rmax+1)
	#plt.plot(r, realq, 'ko')
	plt.errorbar(r, realq, yerr=realsig, fmt="ko")
	plt.plot(r, map(sum, zip(q, [2*x for x in sig])) , 'b--', alpha=0.5)
	plt.plot(r,  map(sum, zip(q, [-2*x for x in sig])), 'b--', alpha=0.5)

	#plt.fill_between(r, map(sum, zip(q, [2*x for x in sig])) , map(sum, zip(q, [-2*x for x in sig])), color="#abbeff",	alpha = 0.85, label="sig2")
	plt.fill_between(r, map(sum, zip(q, [2*x for x in sig])) , map(sum, zip(q, sig)), color="#abbeff", alpha = 0.8, label="asdf")
	plt.fill_between(r, map(sum, zip(q, [-x for x in sig])) , map(sum, zip(q, [-2*x for x in sig])), color="#abbeff", alpha = 0.8, label="asdf")

	plt.fill_between(r, map(sum, zip(q, sig)), map(sum, zip(q, [-x for x in sig])), color="#b55f63", alpha = 0.7, label="sig1")

	plt.xlim(r[0],r[-1])
	limm = 5e-5
	plt.ylim(-limm,limm)
	plt.xlabel("R (deg)")
	plt.ylabel("Q(R)")
	plt.ticklabel_format(style="sci", axis="y", scilimits=(0,0))

def main():
	outdir = "output/"
	N = -1
	avg_q = myavgclass()
	avg_sigma = myavgclass()

	real_q = myavgclass()
	real_sigma = myavgclass()

	for dir_path, dir_names, file_names in os.walk(outdir):		# walk() returns three values always. 
		print "dir_path = \t%s"  %dir_path 		#now in a directory, ex. out_0001
		for single_file in file_names:			#loop over files in directory
			if single_file.endswith('.txt'):		#make sure we extract from text files only
				print "single_file = \t%s"  %single_file	
				temppath = dir_path + "/" + single_file
				file_reader = open(temppath, 'r')	
				temparr = []						#save data from each text file
				for line in file_reader:
					val = float(line.strip())
					temparr.append(val)

				if single_file.startswith("q_e10_20"):
					for i in range(0, Rmax):
							avg_q.e10_20[i] += temparr[i]
				elif single_file.startswith("q_e10_30"):
					for i in range(0, Rmax):
							avg_q.e10_30[i] += temparr[i]
				elif single_file.startswith("q_e10_40"):
					for i in range(0, Rmax):
							avg_q.e10_40[i] += temparr[i]
				elif single_file.startswith("q_e20_30"):
					for i in range(0, Rmax):
							avg_q.e20_30[i] += temparr[i]
				elif single_file.startswith("q_e20_40"):
					for i in range(0, Rmax):
							avg_q.e20_40[i] += temparr[i]
				elif single_file.startswith("q_e30_40"):
					for i in range(0, Rmax):
							avg_q.e30_40[i] += temparr[i]
			########################################################
				elif single_file.startswith("sigma_e10_20"):
					for i in range(0, Rmax):
							avg_sigma.e10_20[i] += temparr[i]
				elif single_file.startswith("sigma_e10_30"):
					for i in range(0, Rmax):
							avg_sigma.e10_30[i] += temparr[i]
				elif single_file.startswith("sigma_e10_40"):
					for i in range(0, Rmax):
							avg_sigma.e10_40[i] += temparr[i]
				elif single_file.startswith("sigma_e20_30"):
					for i in range(0, Rmax):
							avg_sigma.e20_30[i] += temparr[i]
				elif single_file.startswith("sigma_e20_40"):
					for i in range(0, Rmax):
							avg_sigma.e20_40[i] += temparr[i]
				elif single_file.startswith("sigma_e30_40"):
					for i in range(0, Rmax):
							avg_sigma.e30_40[i] += temparr[i]		
		N +=1 
		print N
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

	file10_20= open("output/diffuse/q_e10_20.txt", "r")
	k=0
	for line in file10_20:
		val = float(line.strip())
		real_q.e10_20[k] = val
		k+=1
	#print real_q.e10_20

	k=0
	file10_30 = open("output/diffuse/q_e10_30.txt", "r")
	for line in file10_30:
		val = float(line.strip())
		real_q.e10_30[k] = val
		k+=1

	k=0
	file10_40 = open("output/diffuse/q_e10_40.txt", "r")
	for line in file10_40:
		val = float(line.strip())
		real_q.e10_40[k] = val
		k+=1

	k=0
	file20_30 = open("output/diffuse/q_e20_30.txt", "r")
	for line in file20_30:
		val = float(line.strip())
		real_q.e20_30[k] = val
		k+=1

	k=0
	file20_40 = open("output/diffuse/q_e20_40.txt", "r")
	for line in file20_40:
		val = float(line.strip())
		real_q.e20_40[k] = val
		k+=1


	k=0
	file30_40 = open("output/diffuse/q_e30_40.txt", "r")
	for line in file30_40:
		val = float(line.strip())
		real_q.e30_40[k] = val
		k+=1

		################################################
	file10_20= open("output/diffuse/sigma_e10_20.txt", "r")
	k=0
	for line in file10_20:
		val = float(line.strip())
		real_sigma.e10_20[k] = val
		k+=1
	#print real_sigma.e10_20

	k=0
	file10_30 = open("output/diffuse/sigma_e10_30.txt", "r")
	for line in file10_30:
		val = float(line.strip())
		real_sigma.e10_30[k] = val
		k+=1

	k=0
	file10_40 = open("output/diffuse/sigma_e10_40.txt", "r")
	for line in file10_40:
		val = float(line.strip())
		real_sigma.e10_40[k] = val
		k+=1

	k=0
	file20_30 = open("output/diffuse/sigma_e20_30.txt", "r")
	for line in file20_30:
		val = float(line.strip())
		real_sigma.e20_30[k] = val
		k+=1

	k=0
	file20_40 = open("output/diffuse/sigma_e20_40.txt", "r")
	for line in file20_40:
		val = float(line.strip())
		real_sigma.e20_40[k] = val
		k+=1


	k=0
	file30_40 = open("output/diffuse/sigma_e30_40.txt", "r")
	for line in file30_40:
		val = float(line.strip())
		real_sigma.e30_40[k] = val
		k+=1

	fig = plt.figure(1)
	fig.set_figheight(8)
	fig.set_figwidth(13)
	plt.clf()

	plt.subplot(231)
	make_brazil(avg_q.e10_20, avg_sigma.e10_20, real_q.e10_20, real_sigma.e10_20)
	plot1, = plt.plot([], [], color='#abbeff', alpha = 0.85, linewidth=10, label="1 sig")
	plot2, = plt.plot([], [], color='#b55f63',alpha = 0.7, linewidth=10, label="2 sig")

	plt.title("{10,20} GeV")
	
	plt.legend(loc="upper left")

	plt.subplot(232)
	make_brazil(avg_q.e10_30, avg_sigma.e10_30, real_q.e10_30, real_sigma.e10_30)
	plt.title("{10,30} GeV")

	plt.subplot(233)
	make_brazil(avg_q.e10_30, avg_sigma.e10_40, real_q.e10_40, real_sigma.e10_40)
	plt.title("{10,40} GeV")

	plt.subplot(234)
	make_brazil(avg_q.e20_30, avg_sigma.e20_30, real_q.e20_30, real_sigma.e20_30)
	plt.title("{20,30} GeV")

	plt.subplot(235)
	make_brazil(avg_q.e20_40, avg_sigma.e20_40, real_q.e20_40, real_sigma.e20_40)
	plt.title("{20,40} GeV")

	plt.subplot(236)
	make_brazil(avg_q.e30_40, avg_sigma.e30_40, real_q.e30_40, real_sigma.e30_40)
	plt.title("{30,40} GeV")

	plt.rcParams.update({'figure.autolayout': True})
	plt.show()

