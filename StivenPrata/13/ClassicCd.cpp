#include "ClassicCd.h"
#include <cstring>
#include <iostream>

ClassicCd::ClassicCd(const char * s1, const char * s2, const char * s3, int n, double x): Cd(s1, s2, n, x)
{
	strncpy(mainTheme, s3, 49);
	mainTheme[49] = '\0';
}

ClassicCd::ClassicCd()
{
	mainTheme[0] = '\0';
}

void ClassicCd::Report() const // выводит все данные о компакт-диске
{
	Cd::Report();
	std::cout << "mainTheme: " << mainTheme << std::endl;
}

ClassicCd & ClassicCd::operator= (const ClassicCd & cd)
{
	if(this == &cd)
		return *this;

	Cd::operator=(cd);

	strncpy(mainTheme, cd.mainTheme, 49);
	mainTheme[49] = '\0';

	return *this;
}