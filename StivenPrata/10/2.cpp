#include <iostream>
#include <string>

using namespace std;

class Person {
private:
	static const int LIMIT = 25;
	string lname; // фамилия
	char fname [LIMIT] ; // имя
public:
	Person () { lname = ""; fname[0] = '\0'; } // #1
	Person (const string & ln, const char * fn = "Heyyou"); // #2
	// Следующие методы отображают lname и fname 
	void Show() const; // формат: имя фамилия
	void FormalShow() const; // формат: фамилия, имя
};

Person::Person(const string & ln, const char * fn)
{
	lname = ln;
	for (int i = 0; i < LIMIT; ++i)
	{
		if (i == LIMIT - 1)
		{
			fname[i] = '\0';
			break;
		}
		fname[i] = fn[i];
		if (!fn[i])
			break;

	}
}

void Person::Show() const
{
	cout << fname << ' ' << lname << endl;
}

void Person::FormalShow() const
{
	cout << lname << ',' << ' ' << fname << endl;
}

int main()
{
	Person one;							// используется конструктор по умолчанию
	Person two("Smythecraft");			// используется конструктор #2
										//с одним аргументом по умолчанию
	Person three("Dimwiddy", "Sam");	// используется конструктор #2,
										// без аргументов по умолчанию
	cout << "one: " << endl;
	one.Show(); one.FormalShow();
	cout << endl << "two: " << endl;
	two.Show(); two.FormalShow();
	cout << endl << "three: " << endl;
	three.Show(); three.FormalShow();
	return 0;
}
