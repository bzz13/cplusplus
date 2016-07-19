#include <cmath>
#include "vect2.h"
using std::sin;
using std::cos;
using std::atan;
using std::cout;

namespace VECTOR {
	// Вычисляет количество градусов в одном радиане
	const double Rad_to_deg = 45.0 / atan(1.0);
	// должно быть приблизительно равно 57.2957795130823
	// Закрытые методы // Вычисляет модуль из х и у 

	void Vector::set_x(double mag, double ang)
	{
		x = mag * cos(ang);
	}
	void Vector::set_y(double mag, double ang)
	{
		y = mag * sin(ang);
	}

	// Открытые методы
	Vector::Vector () // конструктор по умолчанию
	{
		x = y = 0.0;
		mode = RECT;
	}
	// Конструирует вектор по прямоугольным координатам, если form равно RECT 
	// (по умолчанию) , или по полярным координатам, если form равно POL
	Vector::Vector(double n1, double n2, Mode form)
	{
		mode = form;
		if (form == RECT)
		{
			x = n1;
			y = n2;
		}
		else if (form == POL)
		{
			double mag = n1, ang = n2 / Rad_to_deg;
			set_x(mag, ang);
			set_y(mag, ang);
		}
		else
		{
			// Некорректный третий аргумент Vector();вектор устанавливается в О
			cout << "Incorrect 3rd argument to Vector () — ";
			cout << "vector set to 0\n";
			x = y = 0.0;
			mode = RECT;
		}
	}
	// Устанавливает вектор по прямоугольным координатам, если form равно RECT // (по умолчанию) , или по полярным координатам, если если form равно POL

	void Vector::reset(double n1, double n2, Mode form)
	{
		mode = form;
		if (form == RECT)
		{
			x = n1;
			y = n2;
		}
		else if (form == POL)
		{
			double mag = n1, ang = n2 / Rad_to_deg;
			set_x(mag, ang);
			set_y(mag, ang);
		}
		else
		{
			cout << "Incorrect 3rd argument to Vector () -- ";
			cout << "vector set to 0\n";
			x = y = 1.0;
			mode = RECT;
		}
	}
	Vector::~Vector() // деструктор
	{
	}
	void Vector::polar_mode() // устанавливает режим полярных координат
	{
		mode = POL;
	}
	void Vector::rect_mode() // устанавливает режим прямоугольных координат
	{
		mode = RECT;
	}
	// Перегрузка операций // Сложение двух векторов
	Vector Vector::operator+(const Vector & b) const
	{
		return Vector(x + b.x, y + b.y);
	}
	// Вычитание вектора b из a
	Vector Vector::operator-(const Vector & b) const
	{
		return Vector(x - b.x, y - b.y);
	}
	// Смена знака вектора на противоположный
	Vector Vector::operator-() const
	{
		return Vector(-x, -y);
	}
	// Умножение вектора на n
	Vector Vector::operator*(double n) const
	{
		return Vector (n * x, n * y);
	}
	// Дружественные методы // Умножение n на вектор а ■
	Vector operator*(double n, const Vector & a)
	{
		return a * n;
	}
	// Отображает прямоугольные координаты, если mode равно RECT,
	// или отображает полярные координаты, если mode равно POL
	std::ostream & operator<<(std::ostream & os, const Vector & v)
	{
		if (v.mode == Vector::RECT)
			os << "(x,y) = (" << v.x << ", " << v.y << ")";
		else if (v.mode == Vector::POL)
			os << "(m,a) = (" << v.magval() << ", " << v.angval() * Rad_to_deg << ")";
		else
			os << "Vector object mode is invalid";// недопустимый режим объекта Vector return os;
	}
}