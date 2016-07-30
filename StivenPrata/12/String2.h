#include <iostream>
#ifndef STRNG_H_
#define STRNG_H_

class String {
private:
	char * str;					//указатель на строку
	int len;					//длина строки
	static int num_strings;		//количество объектов
public:
	String(const char * s);		//конструктор
	String();					//конструктор по умолчанию
	~String();					//деструктор
	String(const String& sb);

	String& operator= (const String&);
	bool operator== (const String&);

	//case operations
	void stringup();
	void stringlow();
	int has(char c);

	//Дружественная функция
	friend std::ostream & operator<< (std::ostream & os, const String & st);
	friend std::istream & operator>> (std::istream & is, String & st);

	friend String operator+ (const String&, const String&);
};
#endif