#ifndef _GOLF_H_
#define _GOLF_H_

class golf
{
	static const int Len = 40;
	char fullname[Len];
	int handicap;
public:
	// Интерактивная версия:
	// функция предлагает пользователю ввести имя и гандикап
	golf();
	// Неинтерактивная версия:
	// функция заполняет имя игрока и его гандикап (фору),
	// используя передаваемые ей аргументы
	golf(const char * name, int he);

	// Функция устанавливает новое значение гандикапа
	void sethandicap (int he);

	// Функция отображает содержимое структуры типа golf
	void show() const;
};

#endif