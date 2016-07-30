#include "StringBad.h"
using namespace std;

void f1(StringBad sb)
{
	cout << "call f1: " << sb << endl;
}
void f2(StringBad &sb)
{
	cout << "call f2: " << sb << endl;
}

StringBad generate(const char * c_str)
{
	return StringBad(c_str);
}

int main()
{
	// {
	// 	StringBad s = StringBad("copy constructor?");
	// 	cout << "s: " << s << endl;
	// }
	{
		cout << "test rvo" << endl;
		StringBad sb = generate("generated string");
		cout << "destructors will be auto call" << endl;
		StringBad other;
		other = sb;
	}
	return 0;
	// {
	// 	cout << "create on stack" << endl;
	// 	StringBad a, b("1231"), c{}, d = {"2423"}, e = "qwe";
	// 	cout << "destructors will be auto call" << endl;
	// 	{
	// 		int a = 5;
	// 		cout << a << " ";
	// 	}
	// 	cout << a << endl;
	// }
	// {
	// 	cout << endl << "create on heap" << endl;
	// 	StringBad *a, *b, *c;
	// 	a = new StringBad;
	// 	b = new StringBad("1231");
	// 	c = new StringBad();
	// 	delete a;
	// 	delete b;
	// 	delete c;
	// 	cout << endl;
	// }
	// {
	// 	cout << "create on heap and lose memory" << endl;
	// 	StringBad *a;
	// 	a = new StringBad;
	// 	a = new StringBad("1231");
	// 	a = new StringBad();

	// 	delete a;
	// 	delete new StringBad("1");
	// 	cout << endl;
	// }
	// {
	// 	cout << "create on stack with copy" << endl;
	// 	StringBad original("Celery Stalks at Midnight");
	// 	StringBad copy1 = original;
	// 	StringBad copy2;
	// 	copy2 = original;

	// 	f1(original);
	// 	f2(original);
	// 	f1(copy1);
	// 	f2(copy1);
	// 	f1(copy2);
	// 	f2(copy2);

	// 	cout << "destructors will be auto call" << endl;
	// }
}