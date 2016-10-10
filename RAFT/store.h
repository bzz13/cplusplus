#ifndef _store_h_
#define _store_h_

#include "logger.h"
#include "operation.h"
#include <string>
#include <unordered_map>
#include <stdexcept>

template<typename TK, typename TV>
class store
{
	std::unordered_map<TK, TV> data;
	logger<operation<TK, TV>> log;

	void apply(const operation<TK, TV>& o);
public:
	store(const 		char* logpath, bool restore = false);
	store(const  std::string& logpath, bool restore = false);

	TV& get(const TK& tk);
	void set(const TK& tk, const TV& tv);
	void del(const TK& tk);

	void showStore();
};

template<typename TK, typename TV>
store<TK, TV>::store(const char* logpath, bool restore): store(std::string(logpath), restore)
{
}

template<typename TK, typename TV>
store<TK, TV>::store(const std::string& logpath, bool restore): log(logpath, restore)
{
	if(restore)
	{
		for (int i = 0; i < log.maxIndex(); ++i)
		{
			apply(log.get(i));
		}
	}
}

template<typename TK, typename TV>
void store<TK, TV>::apply(const operation<TK, TV>& op)
{
	switch(op.type())
	{
		case opertaion_type::set:
			data[op.key()] = op.val();
			break;
		case opertaion_type::del:
			data.erase(op.key());
			break;
		default:
			throw std::logic_error("undef opertaion_type");
	}
}

template<typename TK, typename TV>
TV& store<TK, TV>::get(const TK& tk)
{
	return data[tk];
}

template<typename TK, typename TV>
void store<TK, TV>::set(const TK& tk, const TV& tv)
{
	operation<TK, TV> o(opertaion_type::set, tk, tv);
	log.app(o);
	apply(o);
}

template<typename TK, typename TV>
void store<TK, TV>::del(const TK& tk)
{
	TV tv;
	operation<TK, TV> o(opertaion_type::del, tk, tv);
	log.app(o);
	apply(o);
}

template<typename TK, typename TV>
void store<TK, TV>::showStore()
{
	for (auto item: data)
	{
		std::cout << item.first << ": " << item.second << std::endl;
	}
}

#endif