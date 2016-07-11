#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Patron
{
	string name;
	double value;
};

int main()
{
	int count;

	ifstream fin;
	fin.open("input.txt");

	fin >> count;
	fin.get();

	Patron * members = new Patron[count];

	for (int i = 0; i < count; ++i)
	{
		getline(fin, members[i].name);

		fin >> members[i].value;
		fin.get();
	}

	fin.close();

	cout << "Grand Patrons" << endl;
	bool hasmembers = false;
	for (int i = 0; i < count; ++i)
	{
		if (members[i].value >= 10000)
		{
			hasmembers = true;
			cout << members[i].name << " -> " << members[i].value << endl;
		}
	}
	if (!hasmembers)
		cout << "none" << endl;

	cout << endl << "Patrons" << endl;
	hasmembers = false;
	for (int i = 0; i < count; ++i)
	{
		if (members[i].value < 10000)
		{
			hasmembers = true;
			cout << members[i].name << " -> " << members[i].value << endl;
		}
	}
	if (!hasmembers)
		cout << "none" << endl;

	delete [] members;
}