import matplotlib.pyplot as plt
import numpy as np

Rmax = 30

class myavgclass:
	def __init__(self):
		self.e10_20 = np.zeros(Rmax)
		self.e10_30 = np.zeros(Rmax)
		self.e10_40 = np.zeros(Rmax)
		self.e20_30 = np.zeros(Rmax)
		self.e20_40 = np.zeros(Rmax)
		self.e30_40 = np.zeros(Rmax)


def main():
	common_dir = "averaged_results/"
	q_r = []
	sig_r = []
	r = np.arange(1,31)

	avg_q = myavgclass()
	avg_sigma = myavgclass()

	file = open(common_dir + "q_e10_40.txt", "r")
	for line in file:
		val = float(line.strip())
		q_r.append(val)


	file2 = open(common_dir + "sigma_e10_40.txt", "r")
	for line in file2:
		val = float(line.strip())
		sig_r.append(val)

	plt.clf()
	plot0, = plt.plot(r, q_r, 'ko', label='pooop')

	plt.plot(r, map(sum, zip(q_r, [2*x for x in sig_r])) , 'b--', alpha=0.5)
	plt.plot(r,  map(sum, zip(q_r, [-2*x for x in sig_r])), 'b--', alpha=0.5)

	plt.fill_between(r, map(sum, zip(q_r, [2*x for x in sig_r])) , map(sum, zip(q_r, [-2*x for x in sig_r])), color="#abbeff", alpha = 0.85, label="asdf")
	#plt.fill_between(r, map(sum, zip(q_r, [2*x for x in sig_r])) , map(sum, zip(q_r, sig_r)), color="#abbeff", alpha = 0.85, label="asdf")
	#plt.fill_between(r, map(sum, zip(q_r, [-x for x in sig_r])) , map(sum, zip(q_r, [-2*x for x in sig_r])), color="#abbeff", alpha = 0.85, label="asdf")
	plt.fill_between(r, map(sum, zip(q_r, sig_r)), map(sum, zip(q_r, [-x for x in sig_r])), color="#b55f63", alpha = 0.7)

	plot1, = plt.plot([], [], color='#abbeff', alpha = 0.85, linewidth=10, label="1 sig")
	plot2, = plt.plot([], [], color='#b55f63',alpha = 0.7, linewidth=10, label="2 sig")

	plt.legend()
	plt.show()
	

