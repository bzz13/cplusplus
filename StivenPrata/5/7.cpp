#include <iostream>
#include <string>
using namespace std;


struct car
{
	string name;
	unsigned year;

	const void print()
	{
		cout << year << " " << name << endl;
	}
};

int main()
{
	int count;
	cout << "car count? ";
	cin >> count;

	car * collection = new car[count];

	for (int i = 0; i < count; ++i)
	{
		cout << "car #" << (i+1) << " : " << endl;
		cout << "Enter year: ";
		cin >> collection[i].year;
		cin.get();
		cout << "Enter name: ";
		getline(cin, collection[i].name);
	}

	cout << endl << "collection: " << endl;
	for (int i = 0; i < count; ++i)
		collection[i].print();

	delete [] collection;
}