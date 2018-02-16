#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "cpmain_iterator.h"
#include "mycolors.h"
using namespace std;


int main()
{
	string out_dir_prefix = "output/out_";
	int start = 200;
	int end = 220;
	for (int i=start; i<end; i++)
	{
		int missing_zeros = 4 - to_string(i).length();
		string num_dir = out_dir_prefix + string(missing_zeros, '0') + to_string(i) + "/";
		mkdir((num_dir).c_str(), ACCESSPERMS);
		//cout << num_dir << endl;
		//printf("dick");
		printf("\n\t%s\t New directory created: %s\n", red_string("master_iterator.cpp -->").c_str(), yellow_string(num_dir).c_str());
		cpmain_iterator(num_dir);
	}
}

