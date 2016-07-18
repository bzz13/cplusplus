#include <iostream>
using namespace std;

struct customer
{
	char fullname[35];
	double payment;
};

typedef customer Item;

class Stack {
private:
	enum {MAX = 10}; // константа, специфичная для класса
	Item items[MAX]; // хранит элементы стека
	int top; // индекс вершины стека
public:
	Stack();
	bool isempty() const;
	bool isfull() const;

	// push () возвращает false, если стек полон, и true - в противном случае 
	bool push(const Item & item); // добавляет элемент в стек
	// pop () возвращает false, если стек пуст, и true - в противном случае 
	bool pop(Item & item); // выталкивает элемент с вершины стека
};

Stack::Stack() // создание пустого стека
{
	top = 0;
}
bool Stack::isempty () const {
	return top == 0;
}
bool Stack::isfull () const {
	return top == MAX;
}
bool Stack::push (const Item & item)
{
	if (top < MAX - 1)
	{
		items[top++] = item; 
		return true;
	}
	else
		return false;
}
bool Stack::pop(Item & item)
{
	if (top > 0)
	{
		item = items[--top];
		return true;
	}
	else
		return false;
}

int main()
{
	Stack stack;
	customer cs[4]{
		{"1", 10},
		{"2", 15},
		{"3", 5 },
		{"4", 12.3}
	};

	for (int i = 0; i < 4; ++i)
		stack.push(cs[i]);


	double sum = 0;
	while(!stack.isempty())
	{
		customer c;
		stack.pop(c);
		cout << "poped: " << "n=\"" << c.fullname << "\" p=\"" << c.payment << "\"" << endl;
		sum += c.payment;
	}

	cout << "total payment: " << sum << endl;
}