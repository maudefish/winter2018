#ifndef INV_TRANS_SAMPLE_H
#define INV_TRANS_SAMPLE_H

#include <vector>
#include <cmath>
#include "randnew.h"
//#include "randgauss.h"
#include "histmaker_double.h"
using namespace std;

		/* a) main must include <ctime> and <cstdlib> */
		/* b) seed in main with " srand(time(NULL)*getpid()); " */

void normalize(vector<double>& P_x)		/* normalizes the generated prob. distribution */
{
	double area = 0;
	for (double val : P_x) 
	{
		area += val;
	}

	if (area != 1.)
	{
		for (int i=0, N=P_x.size(); i<N; i++)
		{
			P_x[i] /= area;
		}
	//	cout << "\n\tinv_trans_sample.h: \t P(x) wasn't normalized!! Normed it for you 8^)\n\n";
		printf("\n\t%s\t P(x) wasn't normalized!! Normed it for you 8^)\n", red_string("inv_trans_sample.h -->").c_str());

	}
}


double arbrand(vector<double>& P_x, vector<double>& x)			/* draw a number in the domain of x from your generated P(x) */
{

	double u = randnew(0,1);
	double runningsum = 0;		/* runningsum is the discrete CDF. when it approaches the random value of 'u', the value at x[i-1] is mapped back */
	int i = 0;
	while (runningsum <= u)
	{
		runningsum += P_x[i];
		i++;
	}

	/*double xgauss = randgauss( x[i-1] , ( rangeofx )/( (sizeofx+1)*(sizeofx+1)*20 ) ); 	*/	/* Not sure how stable this is */

	double mynumb;		/* here we take the chosen number and scramble it between the two adjacent bins to give a more continuous output */

	if (i == 1)			/* this set of statements ensures that we don't accidentally reach out of bounds on the histogram */
	{
		mynumb = randnew(x[i-1],x[i]);
		if (mynumb > x[i] || mynumb < x[i-1]) {cout << "bad\n"; cout << i << endl<< mynumb << endl << x[i-1] << endl << endl;}
	}
	else if (i == int(x.size()) )
	{
		mynumb = randnew(x[i-2],x[i-1]);
		if (mynumb > x[i-1] || mynumb < x[i-2]) {cout << "bad\n"; cout << i << endl << x[i-2] << endl << mynumb << endl << x[i-1] << endl << endl;}
	}
	else {
		mynumb = randnew( (x[i-2]+x[i-1])/2 , (x[i-1]+x[i])/2);
		if (mynumb > x[i] || mynumb < x[i-2]) {cout << "bad\n"; cout << i << endl << x[i-2] << endl << mynumb << endl << x[i] << endl << endl;}
	}

	if (abs(mynumb - x[i-1]) > abs(x[1]-x[0])) 
		{
			printf("error -- the generated number is off by more than a bin width\n");
			printf("mapped number = %f\n", x[i-1]);
			printf("scrambled number = %f\n", mynumb);
		}

	return mynumb;
}


vector<double> getP_x(vector<double>& rawdata, int& nbins)	/* creates a Prob. distribution as a normalized set of bins (a histogram) */
{
	vector<double> P_x = histmaker(rawdata, nbins);		/* see histmaker_double.h for more info */
	normalize(P_x);										/* function defined at top */
	return P_x;
}

vector<double> getx(vector<double>& rawdata, int& nbins)	/* creates discrete domain 'x' over which P(x) is defined */
{
	double a = *min_element(rawdata.begin(), rawdata.end());
	double b = *max_element(rawdata.begin(), rawdata.end());

	vector<double> x;
	//cout << a << endl  << b << endl;
	for (int i=0; i<nbins; i++)
	{
		x.push_back (a + i*(b-a)/nbins);
	}
	return x;
}

/* OVERLOAD for when the distrubution is not yet decided  (WARNING: much slower than using the three above functions together) */

/* i.e., you can feed 'arbrand' the raw data and generate P(x) and x with each call using this implementation,,, */
/* but it is much more efficient to generate this two vectors once and then to repeatedly use them with the above non-overload version */

double arbrand(vector<double>& rawdata, int& nbins)
{

	double a = *min_element(rawdata.begin(), rawdata.end());
	double b = *max_element(rawdata.begin(), rawdata.end());

	vector<double> P_x = histmaker(rawdata, nbins);
	vector<double> x;

	for (int i=0, N = P_x.size(); i<N; i++)
	{
		x.push_back ( a + i*b/N );
	}
	
	return arbrand(P_x, x);
}


#endif
