#ifndef MYCOLORS_H
#define MYCOLORS_H

#include <string>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

using namespace std;

string red_string(string mystring)
{
	return ANSI_COLOR_RED + mystring + ANSI_COLOR_RESET;
}

string green_string(string mystring)
{
	return ANSI_COLOR_GREEN + mystring + ANSI_COLOR_RESET;
}

string yellow_string(string mystring)
{
	return ANSI_COLOR_YELLOW + mystring + ANSI_COLOR_RESET;
}

string blue_string(string mystring)
{
	return ANSI_COLOR_BLUE + mystring + ANSI_COLOR_RESET;
}

string magenta_string(string mystring)
{
	return ANSI_COLOR_MAGENTA + mystring + ANSI_COLOR_RESET;
}

string cyan_string(string mystring)
{
	return ANSI_COLOR_CYAN + mystring + ANSI_COLOR_RESET;
}

#endif