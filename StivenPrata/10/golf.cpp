#include <cstring>
#include <iostream>
#include "golf.h"

// Интерактивная версия:
// функция предлагает пользователю ввести имя и гандикап
golf::golf()
{
	using namespace std;
	cout << "Enter name: " << endl;
	cin.getline(fullname, Len - 1);
	cout << "Enter handicap: " << endl;
	cin >> handicap;
}

// Неинтерактивная версия:
// функция заполняет имя игрока и его гандикап (фору),
// используя передаваемые ей аргументы
golf::golf(const char * name, int he)
{
	strncpy(fullname, name, Len - 1);
	handicap = he;
}

// Функция устанавливает новое значение гандикапа
void golf::sethandicap (int he)
{
	handicap = he;
}

// Функция отображает содержимое структуры типа golf
void golf::show() const
{
	using namespace std;
	cout << "Golf { n=\"" << fullname << "\", h=\"" << handicap << "\" }" << endl;
}