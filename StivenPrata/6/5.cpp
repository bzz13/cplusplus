#include <iostream>
#include <string>

using namespace std;

enum step { free5, next10, next20, upper35 };
unsigned int caclTax(unsigned int amount)
{
	int result = 0;
	step current = free5;
	while(amount > 0)
	{
		switch (current)
		{
			case free5:
				amount -= 5000;
				break;
			case next10:
				if (amount < 10000)
					result += amount * 0.1;
				else
					result += 1000;
				amount -= 10000;
				break;
			case next20:
				if (amount < 20000)
					result += amount * 0.15;
				else
					result += 3000;
				amount -= 20000;
				break;
			case upper35:
				result += amount * 0.2;
				amount = 0;
				break;
		}
		current = (step)(current + 1);
	}
	return result;
}

int main()
{
	unsigned int amount;
	cout << "Enter amount: " << endl;
	while(cin >> amount)
		cout << caclTax(amount) << endl;

	return 0;
}