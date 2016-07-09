#include <iostream>
#include <array>

using namespace std;

int main()
{
	array<double, 3> results;
	cout << "Enter results: " << endl;

	cin >> results[0] >> results[1] >> results[2];

	cout << "1: " << results[0] << endl
		 << "2: " << results[1] << endl
		 << "3: " << results[2] << endl
		 << "a: " << (results[0] + results[1] + results[2]) / 3;

	return 0;
}