#include "complex0.h"

complex::complex(double a, double b)
{
	real = a;
	img = b;
}
complex::~complex() { }

complex complex::operator~() const
{
	return complex(real, -img);
}
complex complex::operator+(const complex & c) const
{
	return complex(real + c.real, img + c.img);
}
complex complex::operator-(const complex & c) const
{
	return complex(real - c.real, img - c.img);
}
complex complex::operator*(const complex & c) const
{
	return complex(real*c.real - img*c.img, img*c.real + real*c.img);
}
complex complex::operator*(double d) const
{
	return complex(real*d, img*d);
}

complex operator*(double d, const complex & c)
{
	return c*d;
}
std::ostream & operator<<(std::ostream& out, const complex & c)
{
	using namespace std;
	cout << "(" << c.real << ", " << c.img << "i)";
	return out;
}

std::istream& operator>>(std::istream& in, complex & c)
{
	using namespace std;
	cout << "real: ";
	if (! (cin >> c.real))
		return in;
	cout << "imaginary: ";
	cin >> c.img;
	return in;
}