#include <iostream> 
using namespace std;

#include "ClassicCd2.h"
// будет содержать #incd1ude cd.h

void Bravo(const Cd & disk);

int main()
{
	Cd cd1("Beatles", "Capitol", 14, 35.5);
	ClassicCd cd2 = ClassicCd("Piano Sonata in В flat, Fantasia in C", "Alfred Brendel", "Philips", 2, 57.17);

	Cd *pcd = &cd1;

	// Непосредственное использование объектов 
	cout << "Using object directly:\n";
	cd1.Report(); // использование метода Cd
	cd2.Report(); // использование метода ClassicCd
	cout << endl;

	// Использование указателя на объекты типа cd * 
	cout << "Using type cd * pointer to objects:\n";
	pcd->Report(); // использование метода Cd для объекта cd
	pcd = &cd2;
	pcd->Report(); // использование метода ClassicCd для объекта ClassicCd
	cout << endl;


	// Вызов функции с аргументом-ссылкой на Cd
	cout << "Calling a function with a Cd reference argument:\n";
	Bravo(cd1);
	Bravo(cd2);
	cout << endl;

	// Тестирование присваивания
	cout << "Testing assignment:\n";
	ClassicCd copy; 
	copy = cd2;
	copy.Report();

	return 0;
}

void Bravo(const Cd & disk)
{
	disk.Report();
}