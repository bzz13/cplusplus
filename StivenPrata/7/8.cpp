#include <iostream>
#include <array>
#include <string>

/*
// origin source code

const int Seasons = 4;
const std::array<std::string, Seasons> Snames = {"Spring", "Summer", "Fall", "Winter"};

void fill(std::array<double, Seasons> * pa);
void show(std::array<double, Seasons> da);

int main()
{
	std::array<double, Seasons> expenses;
	fill(&expenses);
	show(expenses);
	return 0;
}

void fill(std::array<double, Seasons> * pa)
{
	using namespace std;
	for (int i = 0; i < Seasons; ++i)
	{
		cout << "Enter " << Snames[i] << " expenses: ";
		cin >> (*pa)[i];
	}
}

void show(std::array<double, Seasons> da)
{
	using namespace std;
	double total = 0.0;
	cout << "EXPENSES" << endl;
	for (int i = 0; i < Seasons; ++i)
	{
		cout << Snames[i] << " $" << da[i] << endl;
		total += da[i];
	}
	cout << "total expenses: $" << total << endl;
}
*/

/*
// subtask a

const int Seasons = 4;
const char * Snames[Seasons] = {"Spring", "Summer", "Fall", "Winter"};

void fill(double * pa);
void show(const double * da);

int main()
{
	double expenses[Seasons]{};
	fill(expenses);
	show(expenses);
	return 0;
}

void fill(double * pa)
{
	using namespace std;
	for (int i = 0; i < Seasons; ++i)
	{
		cout << "Enter " << Snames[i] << " expenses: ";
		cin >> pa[i];
	}
}

void show(const double * da)
{
	using namespace std;
	double total = 0.0;
	cout << "EXPENSES" << endl;
	for (int i = 0; i < Seasons; ++i)
	{
		cout << Snames[i] << " $" << da[i] << endl;
		total += da[i];
	}
	cout << "total expenses: $" << total << endl;
}

*/

// subtask b

const int Seasons = 4;
const char * Snames[Seasons] = {"Spring", "Summer", "Fall", "Winter"};

struct Expanses
{
	double expenses[Seasons];
};

void fill(Expanses * pe);
void show(const Expanses * cpe);

int main()
{
	Expanses expenses;
	fill(&expenses);
	show(&expenses);
	return 0;
}

void fill(Expanses * pa)
{
	using namespace std;
	for (int i = 0; i < Seasons; ++i)
	{
		cout << "Enter " << Snames[i] << " expenses: ";
		cin >> pa->expenses[i];
	}
}

void show(const Expanses * da)
{
	using namespace std;
	double total = 0.0;
	cout << "EXPENSES" << endl;
	for (int i = 0; i < Seasons; ++i)
	{
		cout << Snames[i] << " $" << da->expenses[i] << endl;
		total += da->expenses[i];
	}
	cout << "total expenses: $" << total << endl;
}
