#ifndef _logger_h_
#define _logger_h_

#include <fstream>
#include <string>
#include <stdexcept>
#include <iostream>

template<typename T>
class logger
{
	std::fstream f;
	unsigned int maxIdx = 0;

public:
	logger(const 		char* path, bool restore = false);
	logger(const std::string& path, bool restore = false);
	~logger();

	unsigned int maxIndex() const;

	T get(const unsigned int i);
	void set(const unsigned int i, const T& t);
	void app(const T& t);
};

template<typename T>
logger<T>::logger(const std::string& path, bool restore)
{
	if (restore)
	{
		f.open(path.c_str(), std::ios_base::in | std::ios_base::binary);
		f.seekg(0, std::ios::end);
		maxIdx = f.tellg() / sizeof(T);
		f.close();
		f.open(path.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
	}
	else
	{
		f.open(path.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
	}
}

template<typename T>
logger<T>::logger(const char* path, bool restore): logger(std::string(path), restore)
{
}

template<typename T>
logger<T>::~logger()
{
	f.close();
}

template<typename T>
unsigned int logger<T>::maxIndex() const
{
	return maxIdx;
}

template<typename T>
T logger<T>::get(const unsigned int i)
{
	if (f.is_open()) 
	{
		f.seekg(sizeof(T)*i, f.beg);
		if (!f.fail())
		{
			T tmp;
			f.read((char*) &tmp, sizeof(T));
			f.clear();
			return tmp;
		}
		else
		{
			throw std::out_of_range("cant read from this index");
		}
	}
	else throw std::runtime_error("log file not open");
}

template<typename T>
void logger<T>::set(const unsigned int i, const T& t)
{
	if (f.is_open()) 
	{
		if (i <= maxIdx && !f.seekp(sizeof(T)*i, f.beg).fail())
		{
			f.write((char*) &t, sizeof(T));
			f.flush();
			if (i == maxIdx)
				++maxIdx;
		}
		else throw std::out_of_range("cant write to this index");
	}
	else throw std::runtime_error("log file not open");
}

template<typename T>
void logger<T>::app(const T& t)
{
	set(maxIdx, t);
}

#endif