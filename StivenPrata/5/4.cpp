#include <iostream>
using namespace std;

int main()
{
	double initA = 100, initB = 100;
	double resultA = initA, resultB = initB;

	int year = 0;
	do {
		year++;
		resultA += 0.1  * initA;
		resultB += 0.05 * resultB;

		cout << "After "<< year << " years: " << resultA << " " << resultB << endl;
	}
	while (resultA > resultB);

	return 0;
}