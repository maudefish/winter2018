#ifndef WRITETOTXT_H
#define WRITETOTXT_H
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "mycolors.h"
using namespace std;

/* This program writes data from a vector to a .txt file */
void writetotxt(string filename, const vector<double>& data)
{
	
	filename += ".txt";
	ofstream myscribe(filename);
	for (int i=0, max=data.size(); i<max; i++)
	{
		myscribe << data[i] << "\n";
	}

	/* attempt to open the file to check if it's actually there!! */
	ifstream check(filename);		
	if (check.is_open() == false)		
	{
		printf("\n\t%s\t Unable to write to %s!!\n", red_string("writetotxt.h -->").c_str(), yellow_string(filename).c_str());
	}
	else {printf("\n\t%s\t Data written to %s successfully.\n", red_string("writetotxt.h -->").c_str(), yellow_string(filename).c_str());}
}
 
#endif
