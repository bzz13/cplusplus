#include <string>
#include <iostream>

class Account
{
	std::string holder;
	std::string number;
	double balance;
public:
	Account(const std::string & h, const std::string & n, const double b = 0);
	Account(const char * h, const char * n, double b = 0);
	~Account();

	void show() const;
	void recharge(double a);
	void draw(double a);
};

Account::Account(const std::string & h, const std::string & n, double b)
{
	holder = h;
	number = n;
	if (b < 0)
	{
		std::cout << "Initial account balance should be non negative.\n"
					 "Setup balance to zero." << std::endl;
		b = 0;
	}
	balance = b;
}

Account::Account(const char * h, const char * n, double b)
{
	holder = std::string(h);
	number = std::string(n);
	if (b < 0)
	{
		std::cout << "Initial account balance should be non negative.\n"
					 "Setup balance to zero." << std::endl;
		b = 0;
	}
	balance = b;
}

Account::~Account()
{
	std::cout << "Account was closed" << std::endl;
}

void Account::show() const
{
	using namespace std;
	cout << "{ h=\"" << holder << "\", n=\"" << number << "\", b=\"" << balance << "\" }" << endl;
}

void Account::recharge(double amount)
{
	if (amount < 0)
	{
		std::cout << "Incorrect args: amount should be >= 0. Aborted." << std::endl;
		return;
	}
	balance += amount;
}

void Account::draw(double amount)
{
	if (amount < 0)
	{
		std::cout << "Incorrect args: amount should be >= 0. Aborted." << std::endl;
		return;
	}
	if (amount > balance)
	{
		std::cout << "Incorrect args: amount >= available balance. Aborted." << std::endl;
		return;
	}
	balance -= amount;
}


int main()
{
	{
		Account account {"Card holder", "9532 2342 5433 3211", 1000};
		account.show();
		account.recharge(-1);
		account.recharge(234.1);
		account.show();
		account.draw(-1);
		account.draw(323221);
		account.draw(1200);
		account.show();
	}
	return 0;
}