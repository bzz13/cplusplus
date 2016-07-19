#ifndef _COMPLEX_0_H_
#define _COMPLEX_0_H_

#include <iostream>

class complex
{
	double real;
	double img;
public:
	complex(double a = 0.0, double b = 0.0);
	~complex();

	complex operator~() const;
	complex operator+(const complex & c) const;
	complex operator-(const complex & c) const;
	complex operator*(const complex & c) const;
	complex operator*(double d) const;

	friend complex operator*(double d, const complex & c);
	friend std::ostream& operator<<(std::ostream& out, const complex & c);
	friend std::istream& operator>>(std::istream& out, complex & c);
};

#endif