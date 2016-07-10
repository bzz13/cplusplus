#include <iostream>
#include <array>
using namespace std;

const int arSize = 16;

int main()
{
	array<long long, arSize> factorials { 1 };
	for (int i = 1; i < arSize; ++i)
		factorials[i] = i * factorials[i-1];

	for (int i = 0; i < arSize; ++i)
		cout << i << "! = " << factorials[i] << endl;

	return 0;
}