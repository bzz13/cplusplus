#include <iostream>
#include "complex0.h" // во избежание конфликта c complex.h 
using namespace std;

int main()
{
	complex a (3.0, 4.0); // инициализация значением (3,4i)
	complex c;
	cout << "Enter a complex number (q to quit): " << endl;
	// Ввод комплексного числа (q для завершения)
	while (cin >> c)
	{
		cout << "c is " << c << endl; // значение с
		cout << "complex conjugate is " << ~c << endl; // значение сопряженного числа
		cout << "a is "  << a << endl; // значение а
		cout << "a + c is " << a + c << endl; // значение а + с
		cout << "a — c is " << a - c << endl ; // значение а - с
		cout << "a * c is " << a * c << endl ; // значение а * с
		cout << "2 * c is " << 2 * c << endl ; // значение 2 * с
		cout << "Enter a complex number (q to quit): " << endl;
	}
	cout << "Done!\n"; return 0;
}