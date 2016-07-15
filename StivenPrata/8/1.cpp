#include <iostream>

void sly_print(const char *, int = 0);

int main()
{
	sly_print("first call");
	sly_print("second call");
	sly_print("duble printed string", 2);
	sly_print("forth call", 0);
	sly_print("напечатается 4 раза", 1);
	return 0;
}

int repeater = 0;

void sly_print(const char * cstr, int marker)
{
	using namespace std;
	int count = marker ? repeater : 1;
	for (int i = 0; i < count; ++i)
		cout << cstr << endl;
	repeater ++;
}