#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

class Person
{
	string Name;
	string Surname;
public:
	Person(const string& n, const string& sn): Name(n), Surname(sn) { }
	Person(const char* n, const char* sn): Name(n), Surname(sn) { }

	string GetName() const { return Name; }
	string GetSurname() const { return Surname; }

	virtual void Show() {
		cout << "Name: " << Name << ", Surname: " << Surname << endl;
	}
};

class Gunslinger: public virtual Person
{
	double Time;
	int Count;
public:
	Gunslinger(const string& n, const string& sn, const double& t = 0, const int& c = 0): Person(n, sn), Time(t), Count(c) { }
	Gunslinger(const char* n, const char* sn, const double& t = 0, const int& c = 0): Person(n, sn), Time(t), Count(c) { }

	double Draw() const { return Time; }
	int GetCount() const { return Count; }

	virtual void Show() {
		cout << "Name: " << GetName() << ", Surname: " << GetSurname() << ", Draw: " << Draw() << ", Count: " << Count << endl;
	}
};

class PokerPlayer: public virtual Person
{
public:
	PokerPlayer(const string& n, const string& sn): Person(n, sn) {};
	PokerPlayer(const char* n, const char* sn): Person(n, sn) {};

	int Draw() const { return rand() % 52 + 1; }
};

class BadDude: virtual public Gunslinger, virtual public PokerPlayer
{
public:
	BadDude(const string& n, const string& sn, const double& t = 0, const int& c = 0): Person(n, sn), Gunslinger(n, sn, t, c), PokerPlayer(n, sn) { }
	BadDude(const char* n, const char* sn, const double& t = 0, const int& c = 0): Person(n, sn), Gunslinger(n, sn, t, c), PokerPlayer(n, sn) { }

	double Gdraw() const { return ((Gunslinger*)this)->Draw(); }
	int Cdraw() const { return ((PokerPlayer*)this)->Draw(); }
	virtual void Show() {
		cout << "Name: " << GetName()
			 << ", Surname: " << GetSurname()
			 << ", Gdraw: " << Gdraw()
			 << ", Cdraw: " << Cdraw()
			 << endl;
	}
};

int main()
{
	Person p("qwe", "123");
	p.Show();

	Gunslinger gs("123", "etrte", 4234, 2);
	gs.Show();

	PokerPlayer pp("sfd", "SDF");
	pp.Show();
	cout << pp.Draw() << endl << pp.Draw() << endl << pp.Draw() << endl;

	BadDude bd("b", "d", 12, 234);
	bd.Show();
	cout << bd.Cdraw() << endl << bd.Cdraw() << endl << bd.Cdraw() << endl;

	return 0;
}