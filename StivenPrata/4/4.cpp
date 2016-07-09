#include <iostream>
#include <string>
using namespace std;

int main()
{
	string fn, sn;

	cout << "FN? ";
	getline(cin, fn);
	cout << "SN? ";
	getline(cin, sn);

	cout << sn + ", " + fn;

	return 0;
}