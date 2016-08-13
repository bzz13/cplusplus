#include "Cd2.h"
#include <cstring>
#include <iostream>

Cd::Cd(const char * s1, const char * s2, int n, double x): selections(n), playtime(x)
{
	performers = new char[strlen(s1)+1];
	strcpy(performers, s1);

	label = new char[strlen(s2)+1];
	strcpy(label, s2);
}

Cd::Cd(const Cd & cd)
{
	performers = new char[strlen(cd.performers)+1];
	strcpy(performers, cd.performers);

	label = new char[strlen(cd.label)+1];
	strcpy(label, cd.label);

	selections = cd.selections;
	playtime = cd.playtime;
}

Cd::Cd()
{
	performers = new char[0];
	label = new char[0];

	selections = 0;
	playtime = 0;
}

Cd::~Cd()
{
	delete[] performers;
	delete[] label;
}

void Cd::Report() const // выводит все данные о компакт-диске
{
	using namespace std;
	cout << "performers: " << performers << endl
		 << "label: " << label << endl
		 << "selections: " << selections << endl
		 << "playtime: " << playtime << endl;
}

Cd & Cd::operator= (const Cd & cd)
{
	if (&cd == this)
		return *this;

	delete[] performers;
	delete[] label;

	performers = new char[strlen(cd.performers)+1];
	strcpy(performers, cd.performers);

	label = new char[strlen(cd.label)+1];
	strcpy(label, cd.label);

	selections = cd.selections;
	playtime = cd.playtime;

	return *this;
}
