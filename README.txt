MARCH 30, 2017:

these files are taken from /winter2017/overpopulation

still trying to measure zero. i think i almost have it:
	- data must be flat in solid angle
		- must cut on galactic bulge  (and mask sources)
		- CORRELATOR plots show how inv. trans. sampled data tends towards ideal with cuts 
	- can plot data used with LBplotter.py (each run of cpmain_upperhem_multiphase_savecoords saves the coordinates of the randomly generated positions)

=====================================

JUNE 1, 2017:

(these files are taken from /spring2017 )

the next (final?) step is to make Brazil plots. for this, i’ll need 10000 rounds of generated data

=====================================

JUNE 3, 2017:

now i have MASTER_ITERATOR to run CPMAIN_ITERATOR any number of times, creating new batches of sigmas and Q’s. then, BRAZILPLOTS can scan through all of the output directories and average the values, outputting into averaged_results/ and plotting a set of brazil plots

to make things run quicker while I’m figuring this out:
	- only using upper hem data
	- only using |b|>20 (i.e. maxgamma=70)
	- only letting radius go from (0,20)

would be nice to see how (and if) the error bands tend towards a limit as the number of sample batches increases. Steve said it should approach a nonzero limit (i think)

