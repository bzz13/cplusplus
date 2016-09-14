#include <iostream>
#include <valarray>
#include <utility>
#include <string>

using namespace std;

typedef valarray<int> ArrayInt;
typedef pair<ArrayInt, ArrayInt> PairArray;

class Wine: private string, private PairArray
{
public:
	Wine(): string("noname wine") { };
	Wine(const char * l, int y, const int yr[], const int bot[]) : string(l), PairArray(ArrayInt(yr, y), ArrayInt(bot, y)) { };
	Wine(const char * l, int y): string(l), PairArray(ArrayInt(y), ArrayInt(y)) { };
	~Wine() { };

	void GetBottles() {
		PairArray & bottles = (PairArray &) *this;
		int count = bottles.first.size();
		cout << "Enter " << Label() << " data for " << count << " year(s): " << endl;
		for (int i = 0; i < count; ++i)
		{
			cout << "Enter year: ";
			cin >> bottles.first[i];
			cout << "Enter bottles for that year: ";
			cin >> bottles.second[i];
		}
	}
	const string & Label() const {
		return (string &) *this;
	};
	int Sum() const {
		auto bottles = (PairArray) *this;
		int result = 0;
		for(auto b: bottles.second)
			result += b;
		return result;
	};
	void Show() const{
		auto bottles = (PairArray) *this;
		cout << "Wine: " << Label() << endl
			 << "\tYear\tBottels" << endl;
		for (int i = 0; i < bottles.first.size(); ++i)
			cout << "\t" << bottles.first[i] 
				 << "\t" << bottles.second[i] << endl;
	};
};

int main()
{
	// Wine defaultWine;
	// cout << defaultWine.Sum() << endl;
	// defaultWine.Show();

	cout << "Enter name of wine: ";		// ввод названия вина
	char lab[50];
	cin.getline (lab, 50);
	cout << "Enter number of years: ";	// ввод количества годов сбора винограда
	int yrs;
	cin >> yrs;

	Wine holding(lab, yrs);	// сохранение названия, лет,
							// создание массивов из yrs элементов
	holding.GetBottles();	// предложение ввести год и количество бутылок
	holding.Show();			// вывод содержимого объекта


	// Создание нового объекта, инициализация 
	// с использованием данных из массивов у и Ь 
	const int YRS = 3;
	int y[YRS] = {1993, 1995, 1998};
	int b[YRS] = { 48, 60, 72};

	Wine more("Gushing Grape Red", YRS, y, b);
	more.Show();
	cout << "Total bottles for " << more.Label() // используется метод Label()
		 << ": " << more.Sum() << endl; // используется метод Sum()
	cout << "Bye\n"; return 0;
}