#include <iostream>
#include <cstring>
using namespace std;

class Port // портвейн
{
private:
	char * brand;
	char style[20]; // например, tawny (золотистый), ruby (рубиновый), vintage (марочный)
	int bottles;
public:
	Port(const char * br = "none", const char * st = "none", int b = 0);
	Port(const Port & p); // конструктор копирования
	virtual ~Port() { delete [] brand; }

	Port & operator= (const Port & p);
	Port & operator+= (int b); // добавляет b к bottles
	Port & operator-= (int b); // вычитает b из bottles, если это возможно

	int BottleCount() const { return bottles; }

	virtual void Show() const;
	friend ostream & operator<< (ostream & os, const Port & p);
};

Port::Port(const char * br, const char * st, int b): bottles(b)
{
	brand = new char[strlen(br)+1];
	strcpy(brand, br);

	strncpy(style, st, 19);
	style[19] = '\0';
}

Port::Port(const Port & p)
{
	delete[] brand;
	brand = new char[strlen(p.brand)+1];
	strcpy(brand, p.brand);

	strncpy(style, p.style, 19);
	style[19] = '\0';

	bottles = p.bottles;
}

Port & Port::operator= (const Port & p)
{
	if (this == &p)
		return *this;

	delete[] brand;
	brand = new char[strlen(p.brand)+1];
	strcpy(brand, p.brand);

	strncpy(style, p.style, 19);
	style[19] = '\0';

	bottles = p.bottles;
	return *this;
}

Port & Port::operator+= (int b)
{
	bottles += b;
}

Port & Port::operator-= (int b)
{
	if (bottles >= b)
		bottles -= b;
	else
		cout << "Cant remove " << b << " bottles from " << bottles;
}

void Port::Show() const
{
	cout << "brand: " << brand
		 << "style: " << style
		 << "bottles: " << bottles << endl;
}

ostream & operator<< (ostream & os, const Port & p)
{
	return (os << p.brand << ", " << p.style << ", " << p.bottles);
}


class VintagePort: public Port
{
private:
	char * nickname; // т.е. "The Noble", "Old Velvet" и т.д.
	int year; // год сбора
public:
	VintagePort();
	VintagePort(const char * br, int b, const char * nn, int y);
	VintagePort(const VintagePort & vp);
	virtual ~VintagePort() { delete[] nickname; };

	VintagePort & operator=(const VintagePort & vp);
	void Show() const;
	friend ostream & operator<<(ostream & os, const VintagePort & vp);
};

VintagePort::VintagePort(): Port(), year(0)
{
	nickname = new char[5];
	strncpy(nickname, "none", 4);
	nickname[4] = '\0';
}

VintagePort::VintagePort(const char * br, int b, const char * nn, int y): Port(br, "none", b), year(y)
{
	nickname = new char[strlen(nn)+1];
	strcpy(nickname, nn);
}

VintagePort::VintagePort(const VintagePort & vp): Port(vp)
{
	nickname = new char[strlen(vp.nickname)+1];
	strcpy(nickname, vp.nickname);
}

VintagePort & VintagePort::operator=(const VintagePort & vp)
{
	if (this == &vp)
		return *this;

	Port::operator=(vp);
	delete[] nickname;
	nickname = new char[strlen(vp.nickname)+1];
	strcpy(nickname, vp.nickname);
	year = vp.year;

	return *this;
}

void VintagePort::Show() const
{
	Port::Show();
	cout << "nickname: " << nickname
		 << "year: " << year << endl;
}

ostream & operator<<(ostream & os, const VintagePort & vp)
{
	os << (const Port&) vp;
	return (os << ", " << vp.nickname << ", " << vp.year);
}

int main()
{
	return 0;
}