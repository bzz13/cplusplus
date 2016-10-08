#ifndef _operation_h_
#define _operation_h_

#include <iostream>

enum opertaion_type { undef = 0, set = 1, del = 2 };

template<typename TK, typename TV>
class operation
{
	static unsigned int counter;

	unsigned int idx;
	opertaion_type ttyp;
	TK tkey;
	TV tval;
public:
	operation();
	operation(const opertaion_type& ot, const TK& tk, const TV& tv);

	unsigned int index() const;
	TK key() const;
	TV val() const;
	opertaion_type type() const;

	template<typename FTK, typename FTV>
	friend std::ostream& operator<<(std::ostream& os, const operation<FTK, FTV>& op);
};

template<typename TK, typename TV>
unsigned int operation<TK, TV>::counter = 0;

template<typename TK, typename TV>
operation<TK, TV>::operation(): ttyp(opertaion_type::undef)
{
}

template<typename TK, typename TV>
operation<TK, TV>::operation(const opertaion_type& ot, const TK& tk, const TV& tv): ttyp(ot), tkey(tk), tval(tv)
{
	idx = ++counter;
}

template<typename TK, typename TV>
unsigned int operation<TK, TV>::index() const
{
	return idx;
}

template<typename TK, typename TV>
TK operation<TK, TV>::key() const
{
	return tkey;
}

template<typename TK, typename TV>
TV operation<TK, TV>::val() const
{
	return tval;
}

template<typename TK, typename TV>
opertaion_type operation<TK, TV>::type() const
{
	return ttyp;
}

template<typename TK, typename TV>
std::ostream& operator<<(std::ostream& os, const operation<TK, TV>& op)
{
	os 	<< op.index() 
		<< ": type = " << op.type() 
		<< ", key = " << op.key() 
		<< ", val = {" << op.val() << "}" 
		<< std::endl;
	return os;
}

#endif