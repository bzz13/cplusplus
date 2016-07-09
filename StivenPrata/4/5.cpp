#include <iostream>
#include <string>

using namespace std;

struct CandyBar
{
	string name;
	double weight;
	int kkal;
};

int main()
{
	CandyBar snack {"Mocha Munch", 2.3, 350};

	cout << "{ n = \""   << snack.name
		 << "\", w = \"" << snack.weight
		 << "\", k = \"" << snack.kkal
		 << "\" }" << endl;
	return 0;
}