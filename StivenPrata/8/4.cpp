#include <iostream>
using namespace std;
#include <cstring> // для strlen(), strcpyO

struct stringy {
	char * str;		// указывает на строку
	int ct;			// длина строки (не считая символа * \0 *)
};

// Здесь размещаются прототипы функций set() и show()
void set(stringy & str, const char * cstr);
void show(const stringy & str, int n = 1);
void show(const char * cstr, int n = 1);

int main()
{
	stringy beany;
	char testing [] = "Reality isn't what it used to be.";

	set(beany, testing); // первым аргументом является ссылка,
	// Выделяет пространство для хранения копии testing,
	// использует элемент типа str структуры beany как указатель 
	// на новый блок, копирует testing в новый блок и 
	// создает элемент ct структуры beany
	show(beany);
	show (beany, 2);

	testing[0] = 'D';
	testing [1] = 'u';

	show(testing);
	show(testing, 3);
	show("Done!");

	// TODO: а кто будет освобождать память?
	delete [] beany.str;

	return 0;
}

void set(stringy & str, const char * cstr)
{
	str.ct = strlen(cstr);
	str.str = new char[ str.ct + 1]{}; // not lost '\0' at end. slowly, but shorter
	strcpy(str.str, cstr);
}
void show(const stringy & str, int n)
{
	show(str.str, n);
}
void show(const char * cstr, int n)
{
	for (int i = 0; i < n; ++i)
	{
		cout << cstr << endl;
	}
}
