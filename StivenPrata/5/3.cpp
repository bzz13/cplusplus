#include <iostream>
using namespace std;

const int MaxLength = 100;

int main()
{
	long long current, result = 0;
	do
	{
		cin >> current;
		if (current)
			cout << (result += current) << endl;
	} while(current);

	return 0;
}