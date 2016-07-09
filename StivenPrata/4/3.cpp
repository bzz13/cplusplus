#include <iostream>
#include <cstring>
using namespace std;

const int MaxLength = 100;

int main()
{
	char fn[MaxLength], sn[MaxLength];

	cout << "FN? ";
	cin.getline(fn, MaxLength);
	cout << "SN? ";
	cin.getline(sn, MaxLength);

	char * result = new char[strlen(fn) + strlen(sn) + 3];
	strcpy(result, sn);
	strcpy(result + strlen(sn), ", ");
	strcpy(result + strlen(sn) + 2, fn);

	cout << result;

	delete [] result;

	return 0;
}