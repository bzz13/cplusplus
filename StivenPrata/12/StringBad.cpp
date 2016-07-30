// strngbad.срр -- методы класса StringBad
#include <cstring>// в некоторых случаях — string.h 
#include "StringBad.h"
using std::cout;
// Инициализация статического члена класса
int StringBad::num_strings = 0;

// Методы класса
// Создание StringBad из С-строки
StringBad::StringBad(const char * s)
{
	len = std::strlen (s);							// установка размера
	str = new char[len + 1];						// выделение памяти
	std::strcpy (str, s);							// инициализация указателя
	num_strings++;									// счетчик объектов
	cout << num_strings << ": \"" << str
	     << "\" object created\n";					// для целей отладки
}
StringBad::StringBad ()								// конструктор по умолчанию
{
	len = 4;
	str = new char[4] ;
	std::strcpy(str, "C++");						// строка по умолчанию
	num_strings++;
	cout << num_strings << ": \"" << str
	     << "\" default object created\n";			// для целей отладки
}
StringBad::StringBad(const StringBad& sb)
{
	len = sb.len;
	str = new char[len+1];
	std::strcpy(str, sb.str);
	num_strings++;
	cout << num_strings << ": \"" << str
	     << "\" copy object created\n";			// для целей отладки
}

StringBad::~StringBad()								// необходимый деструктор
{
	cout << "\"" << str << "\" object deleted, ";	// для целей отладки
	--num_strings;									// является обязательным
	cout << num_strings << " left\n";				// для целей отладки
	delete [] str;									// является обязательным
}

StringBad& StringBad::operator= (const StringBad& sb)
{
	if (this != &sb)
	{
		cout << "operator= was called: replace \"" << str << "\" with \"" << sb.str << "\"\n";
		delete[] str;
		len = sb.len;
		str = new char[len+1];
		std::strcpy(str, sb.str);
	}
	return *this;
}

std::ostream & operator<<(std::ostream & os, const StringBad & st)
{
	os << st.str;
	return os;
}