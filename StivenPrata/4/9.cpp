#include <iostream>
#include <string>

using namespace std;

struct CandyBar
{
	string name;
	double weight;
	int kkal;

	const void print()
	{
		cout << "{ n = \"" << name
		 << "\", w = \"" << weight
		 << "\", k = \"" << kkal
		 << "\" }" << endl;
	}
};

int main()
{
	CandyBar
		mm  {"Mocha Munch", 2.3, 350}, 
		ch  {"Chocolate", 1.5, 450},
		mch {"Milk Chocolate", 1.5, 500},
		*goodies;

	goodies = new CandyBar[3] { mm, ch, mch };

	delete [] goodies;

	return 0;
}