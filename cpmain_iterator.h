#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "extract.h"
#include "writetotxt.h"
#include "cpmain_upperhem_multiphase.h"	

/*  default mode of the program includes diffuse and point sources. DIFFUSE and ESOURCE allow ommission of these photons */

#define DIFFUSE true 	/*  no photons within 3 deg. of AGN are counted */
#define E3SOURCE false 	/* all E3 photons replaced with AGN coordinates */

#define maxgamma 70.
#define GAMMA_FOR_ALL true 	/*  ALL photons are constrained within maxgamma */

#define WEIRDSUMS false
/*#include "msum.h"
#include "msum_signed.h"
#include "kahan_sum.h"
#include "kahan_sum_signed.h"
#include "clustersum.h"
#define DEBUG_SUMS  false
#include "vectmerge.h"
#include "magswap.h"
#define SHUFFLE  false 
#include "fisheryates_shuffle.h"*/
#define INVERSE_SAMPLING  true 		/* random E, L, and B will be generated acc. to the real distributions */
#define SOLID_ANGLE_SAMPLING false 	/* (REQUIRES INV. SAMPLING!!) l and b will be pulled from flat and cosine distributions */
#include "inv_trans_sample.h"
#include "randnew.h"
//#include "matplot_call_hist_2vect.h"
using namespace std;

/* NOTE: macro's directory must contain two subdirectories: '\output' (empty) and '\coordinates' (with the L, B, and E .txt files) */
	/* SUBNOTE: matplot_call_hist_2vect.h is required for plotting distributions on the fly */
/* this macro's job is to take 3 files (L, B, and E of the whole upper hemisphere) and compute Q's */

int cpmain_iterator(string out_dir) 
{
	srand(time(NULL)*getpid());

	string coord_dir = "coordinates/";
	string outprefix = out_dir;
	//cout << out_dir << endl;

	string source_dir = "source/";
	//string diffuse_dir_suffix = "diffuse/";
	string diffuse_dir_suffix = "";
	string e3source_dir_suffix = "e3source/";

	string e_filename = coord_dir + "e";
	string l_filename = coord_dir + "l";
	string b_filename = coord_dir + "b";
	vector<double> e = extract(e_filename);
	vector<double> l = extract(l_filename);
	vector<double> b = extract(b_filename);
	vector<double> l_source = extract(coord_dir + source_dir + "l_source");
	vector<double> b_source = extract(coord_dir + source_dir + "b_source");

	#if SHUFFLE == true
		cout << "\n\nSHUFFLE: on\n\n";
		fisheryates(e);
		//fisheryates(l);
		//fisheryates(b);
	#endif

	#if INVERSE_SAMPLING == true
		cout << "\nINVERSE RANDOM SAMPLING: on\n";
		vector<double> e_copy = e;					/* save original data for comparison  */
		vector<double> l_copy = l;
		vector<double> b_copy = b;
		int nbins = 1000;
		vector<double> P_e = getP_x(e,nbins);		/* create the inv. sampling distributions */
		vector<double> P_l = getP_x(l,nbins);
		vector<double> P_b = getP_x(b,nbins);
		vector<double> xp_e = getx(e,nbins);		/* domains to draw from */
		vector<double> xp_l = getx(l,nbins);
		vector<double> xp_b = getx(b,nbins);

		for (int i=0, max=e.size(); i<max; i++)
		{
			e[i] = arbrand(P_e, xp_e);	/* create random data w/ correct distributions */	
			b[i] = arbrand(P_b, xp_b);
			l[i] = arbrand(P_l, xp_l);
		}
		#if SOLID_ANGLE_SAMPLING == true 	
			cout << "\nISOTROPIC (SOLID ANGLE) RANDOM SAMPLING: on\n";
			for (int i=0, max=e.size(); i<max; i++)
			{
				l[i] = randnew(0,360);	
				b[i] = asin(randnew(0,1)*2 - 1)*180./M_PI;		/* CDF for cosine between -pi/2 and pi/2 */
				//b[i] = randnew(-90,90);
			}
			P_b = getP_x(b,nbins);
		#endif

		vector<double> P_e_new = histmaker(e,nbins);
		vector<double> P_e_old = histmaker(e_copy,nbins);
		vector<double> P_l_new = histmaker(l,nbins);
		vector<double> P_l_old = histmaker(l_copy,nbins);
		vector<double> P_b_new = histmaker(b,nbins);
		vector<double> P_b_old = histmaker(b_copy,nbins);

		// matplot(0,60000, 0, 100, P_e_new, xp_e, P_e_old);		/* compare inv. sampled data to actual data */
		// matplot(0,60000, 0, 100, P_l_new, xp_l, P_l_old);		
		// matplot(0,60000, 0, 100, P_b_new, xp_b, P_b_old);		
	#endif


	vector<double> x, y, z;		/* create the unit vectors */ 
	makevectors(l, b, x, y, z);

	vector<double> x_source, y_source, z_source;
	makevectors(l_source, b_source, x_source, y_source, z_source);

	#if GAMMA_FOR_ALL == true
		printf( "\nONLY CONSIDERING PHOTONS WITHIN %2.2f OF ZENITH\n",maxgamma );
	#endif

	#if DIFFUSE == true
		cout << "\nDIFFUSE (MASKING): on\n";
		outprefix += diffuse_dir_suffix;
	#endif

	#if E3SOURCE == true
		cout << "\n\nUSING KNOWN SOURCES AS E3\n\n";
		outprefix += e3source_dir_suffix;
	#endif

	
	newbin bin1, bin2, bin3, bin4, bin5;		/*  bin the data */
	makebins(bin1, bin2, bin3, bin4, bin5, e, x, y, z, l, b, x_source, y_source, z_source);

	vector<double> q_e10_40, q_e10_30, q_e10_20, q_e20_40, q_e20_30, q_e30_40;			/* now we can actually do the statistics */
	vector<double> sigma_e10_40, sigma_e10_30, sigma_e10_20, sigma_e20_40, sigma_e20_30, sigma_e30_40;
	vector<double> n_e10_40, n_e10_30, n_e10_20, n_e20_40, n_e20_30, n_e30_40;
	
	calculateQ(bin1, bin2, bin3, bin4, bin5, q_e10_40, q_e10_30, q_e10_20, q_e20_40, q_e20_30, q_e30_40, sigma_e10_40, sigma_e10_30, sigma_e10_20, sigma_e20_40, sigma_e20_30, sigma_e30_40, n_e10_40, n_e10_30, n_e10_20, n_e20_40, n_e20_30, n_e30_40);

	

	for (int q=0; q<6; q++)			/* write out values of Q to txt files */
	{
		switch (q)
		{
		case 0: 
			writetotxt(outprefix + "q_e10_40", q_e10_40);
			writetotxt(outprefix + "sigma_e10_40", sigma_e10_40);
			writetotxt(outprefix + "n_e10_40", n_e10_40);
			break;
		case 1: 
			writetotxt(outprefix + "q_e10_30", q_e10_30);
			writetotxt(outprefix + "sigma_e10_30", sigma_e10_30);
			writetotxt(outprefix + "n_e10_30", n_e10_30);
			break;
		case 2: 
			writetotxt(outprefix + "q_e10_20", q_e10_20);
			writetotxt(outprefix + "sigma_e10_20", sigma_e10_20);
			writetotxt(outprefix + "n_e10_20", n_e10_20);
			break;
		case 3: 
			writetotxt(outprefix + "q_e20_40", q_e20_40);
			writetotxt(outprefix + "sigma_e20_40", sigma_e20_40);
			writetotxt(outprefix + "n_e20_40", n_e20_40);
			break;
		case 4: 
			writetotxt(outprefix + "q_e20_30", q_e20_30);
			writetotxt(outprefix + "sigma_e20_30", sigma_e20_30);
			writetotxt(outprefix + "n_e20_30", n_e20_30);
			break;
		case 5: 
			writetotxt(outprefix + "q_e30_40", q_e30_40);
			writetotxt(outprefix + "sigma_e30_40", sigma_e30_40);
			writetotxt(outprefix + "n_e30_40", n_e30_40);
			break;
		default: 
			cout << endl; 
			break;
		}		
	}
return 1;}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~end main~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

void fuckoff(const double frick)
{
	cout << frick << endl;
}

double getangle(const double x1, const double y1, const double z1, const double x2, const double y2, const double z2)
{
	return acos( ( x1 * x2 ) + ( y1 * y2 ) + ( z1 * z2 ) ) * 180./M_PI;
}

void makevectors(vector<double>& l, vector<double>& b, vector<double>& x, vector<double>& y, vector<double>& z)
{
	for(int i=0, max=l.size(); i<max; i++)
	{
		x.push_back(cos(l[i]*M_PI/180.)*cos(b[i]*M_PI/180.));
		y.push_back(sin(l[i]*M_PI/180.)*cos(b[i]*M_PI/180.));
		z.push_back(sin(b[i]*M_PI/180.));
	}
}

void makebins(newbin& bin1, newbin& bin2, newbin& bin3, newbin& bin4, newbin& bin5, vector<double>& e, vector<double>& x, vector<double>& y, vector<double>& z, vector<double>& l, vector<double>& b, vector<double>& x_source, vector<double>& y_source, vector<double>& z_source)
{
	double cut1 = 10000;
	double cut2 = 20000;
	double cut3 = 30000;
	double cut4 = 40000;
	double cut5 = 50000;
	double cut6 = 60000;

	vector<double> l_for_plot, b_for_plot;		/* save the coordinates that will be used by analysis */

	fuckoff(b[0]);	/* literally just to avoid unused parameter warnings */
	fuckoff(x_source[0]);
	fuckoff(y_source[0]);
	fuckoff(z_source[0]);
	//cout << endl << endl;

	for (int i=0, max=l.size(); i<max; i++)
	{
		bool goodROI = true;		/* make sure (if DIFFUSE is on) that we use only diffuse data */
		bool goodgamma = true;		/* make sure (if GAMMA_FOR_ALL is on) that we only use photons (all energies) w/in a hard cut angle of zenith */

		#if DIFFUSE == true
			double maskcut = 3.;
			int j = 0;
			int nsources = x_source.size();
			while ( (goodROI == true) && (j < nsources) )
			{
				double theta = getangle(x[i], y[i], z[i], x_source[j], y_source[j], z_source[j]);
				if (theta <= maskcut) {goodROI = false;}			
				j++;
			}
		#endif

		#if GAMMA_FOR_ALL == true 					
			double gamma = acos( z[i] ) * 180./M_PI; 
			if (gamma >= maxgamma) {goodgamma = false; }
		#endif
			
		if ( (goodROI == true) && (goodgamma == true) )		/* only these photons are binned for analysis */
		{

			if ((cut1 <= e[i]) && (e[i] < cut2))
			{
				bin1.x.push_back(x[i]);
				bin1.y.push_back(y[i]);
				bin1.z.push_back(z[i]);
				l_for_plot.push_back(l[i]);			
				b_for_plot.push_back(b[i]);
			}
			else if ((cut2 <= e[i]) && (e[i] < cut3))
			{
				bin2.x.push_back(x[i]);
				bin2.y.push_back(y[i]);
				bin2.z.push_back(z[i]);
				l_for_plot.push_back(l[i]);			
				b_for_plot.push_back(b[i]);
			}
			else if ((cut3 <= e[i]) && (e[i] < cut4))
			{
				bin3.x.push_back(x[i]);
				bin3.y.push_back(y[i]);
				bin3.z.push_back(z[i]);
				l_for_plot.push_back(l[i]);			
				b_for_plot.push_back(b[i]);
			}
			else if ((cut4 <= e[i]) && (e[i] < cut5))
			{
				bin4.x.push_back(x[i]);
				bin4.y.push_back(y[i]);
				bin4.z.push_back(z[i]);
				l_for_plot.push_back(l[i]);			
				b_for_plot.push_back(b[i]);
			}
			else if ((cut5 <= e[i]) && (e[i] < cut6))
			{
				bin5.x.push_back(x[i]);
				bin5.y.push_back(y[i]);
				bin5.z.push_back(z[i]);
				#if E3SOURCE == false
					l_for_plot.push_back(l[i]);			
					b_for_plot.push_back(b[i]);
				#endif
			}
		}
	}

	#if E3SOURCE == true 	/* replaces entire bin of E3 energies with known source coordinates */
		bin5.x.clear();
		bin5.y.clear();
		bin5.z.clear();
		for (int j=0, nsources = x_source.size(); j<nsources; j++)
		{
			bin5.x.push_back(x_source[j]);
			bin5.y.push_back(y_source[j]);
			bin5.z.push_back(z_source[j]);
			l_for_plot.push_back(l[i]);			
			b_for_plot.push_back(b[i]);
		}
	#endif
			
	bin1.size = bin1.x.size();
	bin2.size = bin2.x.size();
	bin3.size = bin3.x.size();
	bin4.size = bin4.x.size();
	bin5.size = bin5.x.size();

	writetotxt("mod_l", l_for_plot);
	writetotxt("mod_b", b_for_plot);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~begin statistics~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

void calculateQ(newbin& bin1, newbin& bin2, newbin& bin3, newbin& bin4, newbin& bin5, vector<double>& q_e10_40, vector<double>& q_e10_30, vector<double>& q_e10_20, vector<double>& q_e20_40, vector<double>& q_e20_30, vector<double>& q_e30_40, vector<double>& sigma_e10_40, vector<double>& sigma_e10_30, vector<double>& sigma_e10_20, vector<double>& sigma_e20_40, vector<double>& sigma_e20_30, vector<double>& sigma_e30_40, vector<double>& n_e10_40, vector<double>& n_e10_30, vector<double>& n_e10_20, vector<double>& n_e20_40, vector<double>& n_e20_30, vector<double>& n_e30_40)
{	
	int maxradius = 20;
	newbin* binA = NULL;
	newbin* binB = NULL;
	newbin* binC = &bin5;
	cout << endl;

	for (int r=0; r<maxradius; r++)
	{

		for (int q=0; q<6; q++)  			/* 6 Q's per radius */
		{	
			int N1 = 0;
			int N2 = 0;
			int N3 = 0;
			double sumQ = 0.;
			double sumQ2 = 0.;
			double sigma = 0.;
			double delta = 0.;

			vector<double> q_for_sum; 

			switch (q)
			{
			case 0: 
				binA = &bin1; binB = &bin4; 
				break;
			case 1: 
				binA = &bin1; binB = &bin3;
				break;
			case 2: 
				binA = &bin1; binB = &bin2;
				break;
			case 3: 
				binA = &bin2; binB = &bin4;
				break;
			case 4: 
				binA = &bin2; binB = &bin3;
				break;
			case 5: 
				binA = &bin3; binB = &bin4;
				break;
			default: 
				break;
			}
			
			for (int k=0; k<(binC->size); k++) 		/* LOOP OVER E3 */
			{
				double cx = binC->x[k];
				double cy = binC->y[k];
				double cz = binC->z[k];

				double gamma = acos( cz ) * 180./M_PI;

				if ( gamma <= maxgamma )		/* 'gamma' is the angle b/w the z-axis and the E3 photon */
				{
					double ax = 0;		/* now that we've chosen a patch with gamma <= R, we can start averaging E1 and E2 positions */
					double ay = 0;		
					double az = 0;
					double bx = 0;
					double by = 0;
					double bz = 0;
					int N1_k = 0;
					int N2_k = 0;

					vector<double> ax_for_sum;	/* only used by WEIRDSUMS */
					vector<double> ay_for_sum;
					vector<double> az_for_sum;
			
					for (int i=0; i<(binA->size); i++)		/* Get avg. position of E1 w/in range of E3 */
					{
						double ax_temp = binA->x[i];
						double ay_temp = binA->y[i];
						double az_temp = binA->z[i];

						double alpha = getangle(ax_temp, ay_temp, az_temp, cx, cy, cz);

						#if DEBUG_SUMS == true
							printf("norm(n_a) = %10.30f\n",ax_temp*ax_temp + ay_temp*ay_temp + az_temp * az_temp );
						#endif

						if (alpha <= r+1)	/* only keep this photon if it's in range */
						{
							ax += ax_temp;
							ay += ay_temp;
							az += az_temp;
							N1_k ++;

							#if WEIRDSUMS == true
								ax_for_sum.push_back(ax_temp);
								ay_for_sum.push_back(ay_temp);
								az_for_sum.push_back(az_temp);
							#endif
							#if DEBUG_SUMS == true
								printf("norm(n_c) = %10.7f\n",ax*ax + ay*ay + az * az );
							#endif
						}
					}

					ax /= N1_k; 	/* because this is an average of ALL E1 within gamma of E3 */
					ay /= N1_k;
					az /= N1_k;

					#if DEBUG_SUMS == true
						printf("\nN1_k = %10i\t norm(n_a) = %10.20f\n", N1_k, ax*ax + ay*ay + az * az );
					#endif

					if (N1_k == 0)		/* prevent infinite values */
					{
						ax = 0;
						ay = 0;
						az = 0;
					}

					for (int j=0; j<(binB->size); j++)		/* get avg. position of E2 w/in range of E3 */
					{
						double bx_temp = binB->x[j];
						double by_temp = binB->y[j];
						double bz_temp = binB->z[j];

						double beta = getangle(bx_temp, by_temp, bz_temp, cx, cy, cz);

						if (beta <= r+1)
						{
							bx += bx_temp;
							by += by_temp;
							bz += bz_temp;
							N2_k ++; 
						}
					}

					bx /= N2_k;
					by /= N2_k;
					bz /= N2_k;
					
					if (N2_k == 0)
					{
						bx = 0;
						by = 0;
						bz = 0;
					} 

					/* calculate Q and Q^2 */

					double tempQ =  ( ax * by * cz ) - ( ax * bz * cy ) - ( ay * bx * cz ) + ( ay * bz * cx ) + ( az * bx * cy ) - ( az * by * cx ) ;

					if (tempQ != 0.)
					{
						q_for_sum.push_back(tempQ);
						sumQ += tempQ;
						sumQ2 += tempQ * tempQ;
						N1 += N1_k;
						N2 += N2_k;
						N3 ++;
					}
				}															/* end loop to check that gamma < maxgamma */ 
			} 																/* end loop over E3 (k)  */

			#if WEIRDSUMS == true
				q_for_sum = vectmerge(q_for_sum);
				double q_msum = msum(q_for_sum) / (N3 );
				double q_msum_signed = msum_signed(q_for_sum) / (N3 );
				double q_kahan = kahan(q_for_sum) / (N3 );
				double q_kahan_signed = kahan_signed(q_for_sum) / (N3 );
				double q_cluster = clustersum(q_for_sum) / (N3 );
			#endif

			sumQ /= N3;
			sumQ2 /= N3;
			sigma = sqrt( (sumQ2 - ( sumQ * sumQ )) / (N3-1.) );
			delta = sigma / sqrt(N3);

			if (N3 == 0) {sumQ = 0; sumQ2 = 0; sigma = 0; delta = 0;}					/* make sure we don't divide by zero */
			if (N3 == 1) {sigma = 0; delta = 0;}

			printf("RADIUS: %i\t #%i\t N1: %i\t N2: %i\t N3: %i\t Q: %10.7g\t σ: %10.7g\t δ: %10.7g\n", r+1, q+1, N1, N2, N3, sumQ, sigma, delta);

			#if WEIRDSUMS == true
				printf("\tregular sum:\t %10.20g\n\tmsum:\t\t %10.20g\n\tmsum signed:\t %10.20g\n\tkahan sum:\t %10.20g\n\tkahan signed\t %10.20g\n\tcluster: \t %10.20g\n\n", sumQ, q_msum, q_msum_signed, q_kahan, q_kahan_signed, q_cluster);
			#endif

			switch (q)
			{
			case 0: 
				q_e10_40.push_back(sumQ);
				sigma_e10_40.push_back(sigma);
				n_e10_40.push_back(N3);
				break;
			case 1: 
				q_e10_30.push_back(sumQ);
				sigma_e10_30.push_back(sigma);
				n_e10_30.push_back(N3);
				break;
			case 2: 
				q_e10_20.push_back(sumQ);
				sigma_e10_20.push_back(sigma);
				n_e10_20.push_back(N3);
				break;
			case 3: 
				q_e20_40.push_back(sumQ);
				sigma_e20_40.push_back(sigma);
				n_e20_40.push_back(N3);
				break;
			case 4: 
				q_e20_30.push_back(sumQ);
				sigma_e20_30.push_back(sigma);
				n_e20_30.push_back(N3);
				break;
			case 5: 
				q_e30_40.push_back(sumQ);
				sigma_e30_40.push_back(sigma);
				n_e30_40.push_back(N3);
				break;
			default: 
				cout << endl; 
				break;
			}			
		}																						/* end loop over q */
		printf("=====================================================================================================================================\n");	
	}	
}

