#ifndef HISTMAKER_DOUBLE_H
#define HISTMAKER_DOUBLE_H
#include <cmath>
#include <vector>
using namespace std;

/* NOTE: To use max and min values of vector 'data' for 'a' and 'b', use 
	a = *min_element(histogram.begin(), histogram.end())
	b = *max_element(histogram.begin(), histogram.end())	*/

vector<double> histmaker(vector<double> data, int& nbins)
{
	double a = *min_element(data.begin(), data.end());
	double b = *max_element(data.begin(), data.end());

	vector<double> myhist(nbins);						/* a set of bins based on input data will be returned */
	
	for (int i=0; i<(int)data.size(); i++)						/* for all entries in data,,, */
	{
		int whichbin = (int)floor(nbins*(data[i]-a)/(b-a));		/* identify which bin the datum corresponds to ,,,*/
		myhist[whichbin] += 1;									/* and increment the value of that bin in the histogram. */
	}
	return myhist;
}
#endif
