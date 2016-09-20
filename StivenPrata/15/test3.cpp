#include <iostream>
using namespace std;

void change(const int* pci, int c){
	int * pi = const_cast<int*>(pci);
	*pi += c;
}

struct dat
{
	short a;
	short b;
};

int main()
{
	int i1 = 600;
	const int i2 = 300;
	cout << "i1, i2 = " << i1 << " " << i2 << endl;
	change(&i1, 10);
	change(&i2, 10);
	cout << "i1, i2 = " << i1 << " " << i2 << endl;

	long val = 0xA224B118;
	auto pd = reinterpret_cast<dat *>(&val);
	cout << hex << pd->a << " " << pd->b << endl;

	return 0;
}