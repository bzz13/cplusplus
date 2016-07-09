#include <iostream>
#include <string>
using namespace std;

int main()
{
	string firstName, secondName;
	int age;
	char grade;
	cout << "FN? ";
	getline(cin, firstName);
	cout << "SN? ";
	getline(cin, secondName);
	cout << "G? ";
	cin >> grade;
	cout << "A? ";
	cin >> age;

	cout << "Name:  " << secondName << ", " << firstName << endl
		 << "Grade: " << (char)(grade+1) << endl
		 << "Age  : " << age;
}