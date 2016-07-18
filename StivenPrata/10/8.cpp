#include <iostream>
#include "list.h"

void Show(Item & item)
{
	std::cout << item << ", ";
}

void Twice(Item & item)
{
	item *= 2;
}

int main()
{
	List l;
	std::cout << "list is empty? " << l.IsEmpty() << std::endl;
	std::cout << "list is full? " << l.IsFull() << std::endl;

	l.Add(1);
	l.Add(2);
	l.Add(3);
	l.Add(4);
	std::cout << "list is empty? " << l.IsEmpty() << std::endl;
	std::cout << "list is full? " << l.IsFull() << std::endl;
	l.ForEach(Show);
	std::cout << std::endl;

	l.Add(5);
	l.Add(6);
	l.Add(7);
	l.Add(8);
	l.Add(9);
	l.Add(10);
	std::cout << "list is empty? " << l.IsEmpty() << std::endl;
	std::cout << "list is full? " << l.IsFull() << std::endl;
	l.ForEach(Show);
	std::cout << std::endl;

	l.Add(11);

	l.ForEach(Twice);
	l.ForEach(Show);
	std::cout << std::endl;

	return 0;
}
