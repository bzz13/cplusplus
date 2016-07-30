// strngbad.срр -- методы класса String
#include <cstring>// в некоторых случаях — string.h 
#include <cctype>
#include "String2.h"
using std::cout;
// Инициализация статического члена класса
int String::num_strings = 0;

// Методы класса
// Создание String из С-строки
String::String(const char * s)
{
	len = std::strlen (s);							// установка размера
	str = new char[len + 1];						// выделение памяти
	std::strcpy (str, s);							// инициализация указателя
	num_strings++;									// счетчик объектов
	// cout << num_strings << ": \"" << str
	//      << "\" object created\n";					// для целей отладки
}
String::String ()								// конструктор по умолчанию
{
	len = 4;
	str = new char[4] ;
	std::strcpy(str, "C++");						// строка по умолчанию
	num_strings++;
	// cout << num_strings << ": \"" << str
	//      << "\" default object created\n";			// для целей отладки
}
String::String(const String& sb)
{
	len = sb.len;
	str = new char[len+1];
	std::strcpy(str, sb.str);
	num_strings++;
	// cout << num_strings << ": \"" << str
	//      << "\" copy object created\n";			// для целей отладки
}

String::~String()								// необходимый деструктор
{
	// cout << "\"" << str << "\" object deleted, ";	// для целей отладки
	--num_strings;									// является обязательным
	// cout << num_strings << " left\n";				// для целей отладки
	delete [] str;									// является обязательным
}

String& String::operator= (const String& sb)
{
	if (this != &sb)
	{
		// cout << "operator= was called: replace \"" << str << "\" with \"" << sb.str << "\"\n";
		delete[] str;
		len = sb.len;
		str = new char[len+1];
		std::strcpy(str, sb.str);
	}
	return *this;
}


bool String::operator== (const String& s)
{
	return strcmp(this->str, s.str) == 0;
}

String operator+ (const String& s1, const String& s2)
{
	char * ss = new char[s1.len + s2.len + 1];
	strcpy(ss, s1.str);
	strcpy(ss + s1.len, s2.str);
	ss[s1.len + s2.len] = '\0';
	String res(ss);
	delete [] ss;
	return res;
}

std::ostream & operator<< (std::ostream & os, const String & st)
{
	os << st.str;
	return os;
}

std::istream & operator>>(std::istream & is, String & st)
{
	char cstr[1000];
	is.getline(cstr, 1000);
	String s(cstr);
	st = s;
	return is;
}

void String::stringup()
{
	for (int i = 0; i < len; ++i)
	{
		str[i] = toupper(str[i]);
	}
}

void String::stringlow()
{
	for (int i = 0; i < len; ++i)
	{
		str[i] = tolower(str[i]);
	}
}

int String::has(char c)
{
	int count = 0;
	for (int i = 0; i < len; ++i)
	{
		if (str[i] == c)
			count ++;
	}
	return count;
}