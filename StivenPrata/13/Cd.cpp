#include "Cd.h"
#include <cstring>
#include <iostream>

Cd::Cd(const char * s1, const char * s2, int n, double x): selections(n), playtime(x)
{
	strncpy(performers, s1, 49);
	performers[49] = '\0';

	strncpy(label, s2, 19);
	label[19] = '\0';
}

Cd::Cd(const Cd & cd)
{
	strncpy(performers, cd.performers, 49);
	performers[49] = '\0';

	strncpy(label, cd.label, 19);
	label[19] = '\0';

	selections = cd.selections;
	playtime = cd.playtime;
}

Cd::Cd()
{
	performers[0] = '\0';
	label[0] = '\0';

	selections = 0;
	playtime = 0;
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

	strncpy(performers, cd.performers, 49);
	performers[49] = '\0';

	strncpy(label, cd.label, 19);
	label[19] = '\0';

	selections = cd.selections;
	playtime = cd.playtime;

	return *this;
}
