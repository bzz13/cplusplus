#include <iostream>
#include <string>

struct CandyBar
{
	std::string name;
	double weight;
	int kkal;
};

void showCandyBar(const CandyBar & bar);
void makeCandyBar(CandyBar & bar, const char * name = "Millennium Munch", double weight = 2.85, int kkal = 350);

int main()
{
	CandyBar bar;
	makeCandyBar(bar, "Moon light", 3.5, 450);
	showCandyBar(bar);

	makeCandyBar(bar, "Black holl", 237492873498.1); // 350 kkal
	showCandyBar(bar);

	makeCandyBar(bar, "Sunrise"); // 2.85 350
	showCandyBar(bar);

	makeCandyBar(bar); // 2.85 350
	showCandyBar(bar);
	return 0;
}

void showCandyBar(const CandyBar & bar)
{
	using namespace std;
	cout << "CandyBar: { name=\"" << bar.name << "\", weight=\"" << bar.weight << "\", kkal=\"" << bar.kkal << "\" }" << endl;
}
void makeCandyBar(CandyBar & bar, const char * name, double weight, int kkal)
{
	bar.name = std::string(name);
	bar.weight = weight;
	bar.kkal = kkal;
}
