#include "iostream"
#include "cstring"
using namespace std;

class Cow {
	char name[20];
	char * hobby;
	double weight;
public:
	Cow();
	Cow(const char * nm, const char * ho, double wt);
	Cow(const Cow &);
	~Cow();
	Cow & operator= (const Cow &);
	void ShowCow() const; // отображение всех данных cow
};

Cow::~Cow()
{

	delete[] hobby;
}

Cow::Cow(): weight(0)
{
	// cout << "default constructor " << endl;

	name[0] = '\0';

	hobby = new char[1];
	hobby[0] = '\0';
}

Cow::Cow(const char * nm, const char * ho, double wt): weight(wt)
{
	// cout << "init constructor " << endl;

	strncpy(name, nm, 20);
	name[19] = '\0';

	hobby = new char[strlen(ho) + 1];
	strcpy(hobby, ho);
}

Cow::Cow(const Cow & c)
{
	// cout << "copy constructor " << endl;

	strcpy(name, c.name);

	hobby = new char[strlen(c.hobby) + 1];
	strcpy(hobby, c.hobby);

	weight = c.weight;
}

Cow & Cow::operator= (const Cow & c)
{
	if ( this == &c )
		return *this;

	delete[] hobby;

	strcpy(name, c.name);

	hobby = new char[strlen(c.hobby)+1];
	strcpy(hobby, c.hobby);

	weight = c.weight;

	return *this;
}

void Cow::ShowCow() const
{
	cout << "Cow: name=\"" << name << "\", hobby=\"" << hobby << "\", weight=\"" << weight << "\"" << endl;
}

int main()
{
	/*
	{
		Cow c1, c2("c2", "hobby of Cow c2", 1);
		c1.ShowCow();
		c2.ShowCow();

		Cow c3 = c1, c4 = c2;
		c3.ShowCow();
		c4.ShowCow();

		c3 = c4;
		c3.ShowCow();

		Cow c5("incredible long name with lenght more then 20 characters", "", 243432);
		c4 = c5;
		c4.ShowCow();
		c5.ShowCow();
	}
	*/
	{
		Cow
			* dc1 = new Cow,
			* dc2 = new Cow("dc2", "hobby of Cow dc2", 12),
			* dc3 = new Cow("234234", "sdfsd", 342);

		dc1->ShowCow();
		dc2->ShowCow();
		dc3->ShowCow();

		*dc1 = *dc2;
		dc1->ShowCow();
		dc2->ShowCow();

		dc1->operator=(*dc3);
		dc1->ShowCow();
		dc3->ShowCow();

		delete dc1;
		delete dc2;
		delete dc3;
	}

	return 0;
}