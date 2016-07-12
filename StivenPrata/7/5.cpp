#include <iostream>
using namespace std;

unsigned long long factorial(unsigned short v);

int main()
{
	unsigned short v;
	cout << "Enter val for factorial calc ( q - for quit): ";
	while (cin >> v)
	{
		cout << factorial(v) << endl
			 << "Enter val for factorial calc ( q - for quit): ";
	}

	return 0;
}

unsigned long long factorial(unsigned short v)
{
	return v ? v * factorial(v-1) : 1;
}