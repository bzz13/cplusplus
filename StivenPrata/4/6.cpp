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
	CandyBar goodies[] {
		{"Mocha Munch", 2.3, 350}, 
		{"Chocolate", 1.5, 450}, 
		{"Milk Chocolate", 1.5, 500}
	};

	goodies[0].print();
	goodies[1].print();
	goodies[2].print();

	return 0;
}