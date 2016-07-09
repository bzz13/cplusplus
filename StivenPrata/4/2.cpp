#include <iostream>
#include <string>
using namespace std;


int main()
{
	string name, dessert;
	cout << "Enter your name: ";
	getline(cin, name);
	cout << "Enter your favorite dessert: ";
	getline(cin, dessert);

	// test raw string
	cout << R"(I have some delicious ")" << dessert
		 << R"*#(" for you, )*#" << name;
	return 0;
}