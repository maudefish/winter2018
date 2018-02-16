from numpy import *
from matplotlib.pyplot import *

def main():
	#fileRA = open("RA.txt", "r")
	#fileDEC = open("DEC.txt", "r")
	#myl = "coordinates/l.txt"
	#myb = "coordinates/b.txt"

	for 

	myl = "mod_l.txt"
	myb = "mod_b.txt"
	lsource = "coordinates/source/l_source.txt"
	bsource = "coordinates/source/b_source.txt"
	fileL = open(myl, "r")
	fileB = open(myb, "r")
	fileLsource = open(lsource, "r")
	fileBsource = open(bsource, "r")

	lcoords = []
	bcoords = []
	lsourcecoords = []
	bsourcecoords = []

	for line in fileL:
		b = float(line.strip())
		lcoords.append(b)

	for line in fileB:
		b = float(line.strip())
		bcoords.append(b)	

	for line in fileLsource:
		b = float(line.strip())
		lsourcecoords.append(b)

	for line in fileBsource:
		b = float(line.strip())
		bsourcecoords.append(b)

	clf()
	plot(lcoords,bcoords, 'k.')
	plot(lsourcecoords,bsourcecoords, 'r*')
	title("Output E3 {L,B} Coordinates")
	xlabel("Galactic Longitude (L)")
	ylabel("Galactice Latitude (B)")
	xlim(0,360)
	ylim(-90,90)

