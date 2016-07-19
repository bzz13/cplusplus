#ifndef VECT0R_H_
#define VECT0R_H_
#include <iostream>
#include <cmath>

using std::sqrt;
using std::atan2;

namespace VECTOR {
	class Vector{
	public:
		enum Mode {RECT, POL};

	private:
		double x;
		double y;
		Mode mode;

		void set_x(double mag, double ang);
		void set_y(double mag, double ang);

	public:
		Vector();
		Vector (double nl, double n2, Mode form = RECT);
		~Vector();

		void reset (double nl, double n2, Mode form = RECT);

		inline double xval() const { return x; }
		inline double yval() const { return y; }
		inline double magval() const { return sqrt(x * x + y * y); }
		inline double angval() const { return (x == 0.0 && y == 0.0) ? 0.0 : atan2 (y, x); }

		void polar_mode();
		void rect_mode ();

		Vector operator+(const Vector & b) const;
		Vector operator-(const Vector & b) const;
		Vector operator-() const;
		Vector operator*(double n) const;

		friend Vector operator*(double n, const Vector & a);
		friend std::ostream & operator<< (std::ostream & os, const Vector & v);
	};
}

#endif