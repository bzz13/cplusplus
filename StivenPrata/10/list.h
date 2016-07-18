#ifndef _LIST_H_
#define _LIST_H_

typedef int Item;

class List
{
	static const int MaxLength = 10;
	Item items[MaxLength];
	int pos;

public:
	List();

	void Add(Item item);
	bool IsEmpty() const;
	bool IsFull() const;
	void ForEach(void (*pf) (Item & item));
};


#endif