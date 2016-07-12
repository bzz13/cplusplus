#include <iostream>

long double probability (unsigned, unsigned);
long double mega_probability (unsigned, unsigned, unsigned, unsigned);

int main()
{
	using namespace std;
	double total = 47, choices = 5, mega_total = 27, mega_choices = 1;

	cout << "You have one chance in "
		 << mega_probability(total, choices, mega_total, mega_choices)
		 << endl;

	return 0;
}

long double probability (unsigned numbers, unsigned picks)
{
	long double result = 1.0, n;
	unsigned p;
	for (n = numbers, p = picks; p > 0; n --, p--)
		result = result * n / p ;
	return result;
}

long double mega_probability(unsigned numbers, unsigned picks, unsigned mega_numbers, unsigned mega_picks)
{
	return probability(numbers, picks) * probability(mega_numbers, mega_picks);
}