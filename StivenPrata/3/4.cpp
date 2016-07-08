#include <iostream>
using namespace std;

void printDate(long long seconds)
{
	auto ss = seconds % 60;
	auto minutes = seconds / 60;
	auto mm = minutes % 60;
	auto hours = minutes / 60;
	auto hh = hours % 60;
	auto days = hours / 24;

	cout << days << " days " << hh << ":" << mm << ":" << ss;
}

int main()
{
	cout << "Enter seconds count: ";
	long long seconds;
	cin >> seconds;
	printDate(seconds);
	return 0;
}