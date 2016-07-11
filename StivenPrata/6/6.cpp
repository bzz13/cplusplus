#include <iostream>
#include <string>

using namespace std;

struct Mecenat
{
	string name;
	double value;
};

int main()
{
	int count;
	cout << "members count: ";
	cin >> count;
	cin.get();

	Mecenat * members = new Mecenat[count];

	for (int i = 0; i < count; ++i)
	{
		cout << "enter name: ";
		getline(cin, members[i].name);

		cout << "enter value: ";
		cin >> members[i].value;
		cin.get();
	}

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