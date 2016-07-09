#include <iostream>
#include <string>
using namespace std;

struct Pizza
{
	string name;
	int diametr;
	int weight;

	const void print()
	{
		cout << "Pizza { n = \"" << name
			 << "\" d = \"" << diametr
			 << "\" w = \"" << weight
			 << "\" }";
	}
};

int main()
{
	Pizza p;

	cout << "name? ";
	getline(cin, p.name);
	cout << "diametr? ";
	cin >> p.diametr;
	cout << "weight? ";
	cin >> p.weight;

	p.print();
}