#include <iostream>

class Plorg
{
	static const int MaxNameLength = 20;
	char name[MaxNameLength];
public:
	int CI;

	Plorg(const char * n = "Plorga");
	void Status() const;
};

Plorg::Plorg(const char * n)
{
	for (int i = 0; i < MaxNameLength; ++i)
	{
		if (i == MaxNameLength - 1)
		{
			name[i] = '\0';
			break;
		}
		name[i] = n[i];
		if (!name[i])
			break;
	}

	CI = 50;
}

void Plorg::Status() const
{
	using namespace std;
	cout << "{ n=\"" << name << "\", CI=\"" << CI << "\" }" << endl;
}


int main()
{
	Plorg p1, p2("preved plorg");
	p1.Status();
	p2.Status();

	p1.CI -= 23;
	p1.Status();

	return 0;
}
