#include <iostream>
#include "list.h"

List::List()
{
	pos = 0;
}

void List::Add(Item item)
{
	if (IsFull())
	{
		std::cout << "list is full. aborted." << std::endl;
		return;
	}
	items[pos++] = item;
}

bool List::IsEmpty() const
{
	return !pos;
}
bool List::IsFull() const
{
	return pos == MaxLength;
}
void List::ForEach(void (*pf) (Item & item))
{
	for (int i = 0; i < pos; ++i)
		pf(items[i]);
}