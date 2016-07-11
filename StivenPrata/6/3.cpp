#include <iostream>
using namespace std;

void printHelp()
{
	cout << "Please enter one of the following choices:" << endl
		 << "c) carnivore" << endl
		 << "g) game" << endl
		 << "p) pianist" << endl
		 << "t) tree" << endl;
}

void printErr()
{
	cout << "Please enter a 'c', 'g', 'p' or 't'" << endl;
}

int main()
{
	printHelp();

	bool complete = false;
	while(!complete)
	{
		char c;
		cin >> c;
		switch (c)
		{
			case 'c': 
			case 'g':
			case 'p':
			case 't':
				cout << endl << "you enter: " << c << endl;
				complete = true;
				break;
			default:
				printErr();
				break;
		}
	}
	return 0;
}