#ifndef CPMAIN_UPPERHEM_MULTIPHASE_H
#define CPMAIN_UPPERHEM_MULTIPHASE_H
#include <string>
#include <vector>
using namespace std;

class newbin
{
public:
	vector<double> e, x, y, z, l, b;
	int size;
};

void makevectors(vector<double>& l, vector<double>& b, vector<double>& x, vector<double>& y, vector<double>& z); 

void makebins(newbin& bin1, newbin& bin2, newbin& bin3, newbin& bin4, newbin& bin5, vector<double>& e, vector<double>& x, vector<double>& y, vector<double>& z, vector<double>& l, vector<double>& b, vector<double>& x_source, vector<double>& y_source, vector<double>& z_source); 

void calculateQ(newbin& bin1, newbin& bin2, newbin& bin3, newbin& bin4, newbin& bin5, vector<double>& q_e10_40, vector<double>& q_e10_30, vector<double>& q_e10_20, vector<double>& q_e20_40, vector<double>& q_e20_30, vector<double>& q_e30_40, vector<double>& sigma_e10_40, vector<double>& sigma_e10_30, vector<double>& sigma_e10_20, vector<double>& sigma_e20_40, vector<double>& sigma_e20_30, vector<double>& sigma_e30_40, vector<double>& n_e10_40, vector<double>& n_e10_30, vector<double>& n_e10_20, vector<double>& n_e20_40, vector<double>& n_e20_30, vector<double>& n_e30_40);


#endif
