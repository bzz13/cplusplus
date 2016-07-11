#include <iostream>
#include <fstream>
using namespace std;

const int MaxLenght = 20;

int main()
{
	int count = 0;
	ifstream fin;
	fin.open("8.cpp");
	while(!fin.eof())
	{
		char c = fin.get();
		cout << char(c);
		count++;
	}
	fin.close();

	cout << endl << "Char coutnt: " << count;
	return 0;
}