#include <cstring>
#include <iostream>
#include "golf.h"

// Неинтерактивная версия:
// функция присваивает структуре .типа golf имя игрока и его гандикап (фору),
// используя передаваемые ей аргументы
void setgolf (golf & g, const char * name, int he)
{
	strncpy(g.fullname, name, Len - 1);
	g.handicap = he;
}

// Интерактивная версия:
// функция предлагает пользователю ввести имя и гандикап,
// присваивает элементам структуры g введенные значения;
// возвращает 1, если введено имя, и 0, если введена пустая строка 
int setgolf(golf & g)
{
	using std::cin;
	using std::cout;
	using std::endl;
	cout << "Enter name: " << endl;
	cin.getline(g.fullname, Len - 1);
	cout << "Enter handicap: " << endl;
	cin >> g.handicap;
	return strcmp(g.fullname, "");
}

// Функция устанавливает новое значение гандикапа
void handicap (golf & g, int he)
{
	g.handicap = he;
}

// Функция отображает содержимое структуры типа golf
void showgolf(const golf & g)
{
	using std::cout;
	using std::endl;
	cout << "Golf { n=\"" << g.fullname << "\", h=\"" << g.handicap << "\" }" << endl;
}