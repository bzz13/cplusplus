#include "ClassicCd2.h"
#include <cstring>
#include <iostream>

ClassicCd::ClassicCd(const char * s1, const char * s2, const char * s3, int n, double x): Cd(s1, s2, n, x)
{
	mainTheme = new char[strlen(s3)+1];
	strcpy(mainTheme, s3);
}

ClassicCd::ClassicCd() : Cd()
{
	mainTheme = new char[0];
}

ClassicCd::~ClassicCd()
{
	delete[] mainTheme;
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

	delete[] mainTheme;
	mainTheme = new char[strlen(cd.mainTheme)+1];
	strcpy(mainTheme, cd.mainTheme);
}