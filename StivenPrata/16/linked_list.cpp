#include <iostream>
#include <string>

template<typename T>
struct node
{
	T value;
	node<T>* next;
	node(const T& rt): value(rt)
	{
		next = nullptr;
	}
};

template<typename T>
class list
{
	node<T>* head;
	node<T>* tail;

private:
	bool callFnToNode(node<T>* ptr_node, void (*func)(node<T>* ptr_node))
	{
		if (ptr_node == nullptr)
			return false;
		func(ptr_node);
		return true;
	}

	void for_each_reverted_order(void (*func)(node<T>* ptr_node), node<T>* tmp)
	{
		if(tmp != nullptr)
		{
			for_each_reverted_order(func, tmp->next);
			callFnToNode(tmp, func);
		}
	}

	list(const list<T>& l);
public:
	list()
	{
		head = tail = nullptr;
	}
	~list()
	{
		while(head != nullptr)
		{
			auto tmp = head->next;
			delete head;
			head = tmp;
		}
	}

	list& add(const T& t)
	{
		if (tail != nullptr)
		{
			tail->next = new node<T>(t);
			tail = tail->next;
		}
		else
			head = tail = new node<T>(t);
		return *this;
	}

	list& for_each(void (*func)(node<T>* ptr_node))
	{
		auto tmp = head;
		while(callFnToNode(tmp, func))
			tmp = tmp->next;
		return *this;
	}

	list& for_each_reverted_order(void (*func)(node<T>* ptr_node))
	{
		for_each_reverted_order(func, head);
		return *this;
	}

	list& chainCall(void (*func)())
	{
		func();
		return *this;
	}

	T* find(const T& t) const
	{
		auto tmp = head;
		while(tmp != nullptr)
		{
			if (tmp->value == t)
				return &(tmp->value);
			tmp = tmp->next;
		}
		return nullptr;
	}
};

template<typename T>
void print_node(node<T>* ptr_node)
{
	std::cout << ptr_node->value << " ";
}

void print_endl()
{
	std::cout << std::endl;
}

int main()
{
	{
		list<int> l;
		l
			.add(1).add(2).add(3).add(4).add(5)
			.for_each(print_node)
			.chainCall([](){ std::cout << std::endl; })
			.for_each_reverted_order(print_node)
			.chainCall(print_endl);
	}
	{
		list<std::string> l;
		l
			.add("qwe")
			.add("asd")
			.for_each(print_node)
			.chainCall(print_endl);

		*(l.find("asd")) = "dsa";

		l
			.for_each_reverted_order(print_node)
			.chainCall(print_endl);
	}

	return 0;
}