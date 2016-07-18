#include <iostream>
using namespace std;

class Move {
private:
	double x;
	double y;
public:
	Move (double a = 0, double b = 0);	// устанавливает x, у в a, b 
	inline void showmove() const;				// отображает текущие значения x, у
	Move add (const Move & m) const;
	// Эта функция добавляет х из m к х вызывающего объекта,
	// чтобы получить новое значение х,
	// Добавляет у из ш к у вызывающего объекта, чтобы получить новое 
	// значение у, присваивает инициализированному объекту значения х, у 
	// и возвращает его
	void reset(double a = 0, double b = 0);
};

Move::Move(double a, double b)
{
	x = a;
	y = b;
}

inline void Move::showmove() const
{
	cout << "X=\"" << x << "\", Y=\"" << y << "\"" << endl;
}

Move Move::add(const Move & m) const
{
	Move result(x + m.x, y + m.y);
	return result;
}

void Move::reset(double a, double b)
{
	x = a;
	y = b;
}

int main()
{
	Move ms[3] {
		Move(), // 0;0
		Move(10), // 10;0
		Move(0, 20), // 0;20
	};

	for (int i = 0; i < 3; ++i)
		ms[i].showmove();

	Move addm = ms[1].add(ms[2]);
	addm.showmove();
	addm.reset(1, 1);
	addm.showmove();
}