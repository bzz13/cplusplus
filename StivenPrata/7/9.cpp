#include <iostream>
#include <cstring>
using namespace std;
const int SLEN = 30;

struct student
{
	char fullname[SLEN];
	char hobby[SLEN];
	int ooplevel;
};

// getinfo() принимает два аргумента: указатель на первый элемент
// массива структур student и значение int, представляющее
// количество элементов в массиве. Функция запрашивает и 
// сохраняет данные о студентах. Ввод прекращается либо после 
// наполнения массива, либо при вводе пустой строки в качестве 
// имени студента. Функция возвращает действительное количество 
// введенных элементов.
int getinfo(student pa[], int n);

// display1() принимает в качестве аргумента структуру student 
// и отображает ее содержимое. 
void display1(student st);

// display2() принимает адрес структуры student в качестве аргумента 
// и отображает ее содержимое, 
void display2(const student * ps);

// display3() принимает указатель на первый элемента массива 
// структур student и количество элементов в этом массиве и 
// отображает содержимое всех структур в массиве, 
void display3(const student pa[], int n);

int main ()
{
	cout << "enter class size: ";
	int class_size;
	cin >> class_size;
	while(cin.get() != '\n')
		continue;
	student *ptr_stu = new student[class_size];
	int entered = getinfo(ptr_stu, class_size);

	for (int i = 0; i < entered; ++i)
	{
		display1(ptr_stu[i]);
		display2(&ptr_stu[i]);
	}
	display3(ptr_stu, entered);

	delete[] ptr_stu;

	cout << "Done!" << endl;

	return 0;
}

int getinfo(student pa[], int n)
{
	int entered = 0;
	for (int i = 0; i < n; ++i, ++entered)
	{
		cout << "enter student #" << (i+1) << " info:" << endl;
		cout << "fullname: ";
		cin.getline(pa[i].fullname, SLEN-1);
		if (strcmp(pa[i].fullname, "") == 0)
			break;
		cout << "hobby: ";
		cin.getline(pa[i].hobby, SLEN-1);
		cout << "ooplevel: ";
		cin >> pa[i].ooplevel;
		cin.get();
	}
	return entered;
}
void display1(student st)
{
	cout << "display1: " 
		 << "{ n=\"" << st.fullname << "\", h=\"" << st.hobby << "\", l =\"" << st.ooplevel << "\"}"
		 << endl;
}
void display2(const student * ps)
{
	cout << "display2: " 
		 << "{ n=\"" << ps->fullname << "\", h=\"" << ps->hobby << "\", l =\"" << ps->ooplevel << "\"}"
		 << endl;
}
void display3(const student pa[], int n)
{
	cout << "display3: " << endl;
	for (int i = 0; i < n; ++i)
	{
		display2(pa+i);
	}
}