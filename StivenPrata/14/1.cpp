#include <iostream>
#include <valarray>
#include <utility>
#include <string>

typedef std::valarray<int> ArrayInt;
typedef std::pair<ArrayInt, ArrayInt> PairArray;

class Wine
{
	std::string label;
	PairArray bottles;
public:
	Wine(): label("noname") { };
	Wine(const char * l, int y, const int yr[], const int bot[]) : label(l), bottles(ArrayInt(yr, y), ArrayInt(bot, y)) { }
	Wine(const char * l, int y): label(l), bottles(ArrayInt(y), ArrayInt(y)) { };
	~Wine() { };

	void GetBottles() {
		using std::cout;
		using std::cin;
		using std::endl;

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
	const std::string & Label() const {
		return label;
	};
	int Sum() const {
		int result = 0;
		for(auto b: bottles.second)
			result += b;
		return result;
	};
	void Show() const{
		using std::cout;
		using std::endl;

		cout << "Wine: " << Label() << endl
			 << "\tYear\tBottels" << endl;
		for (int i = 0; i < bottles.first.size(); ++i)
			cout << "\t" << bottles.first[i] 
				 << "\t" << bottles.second[i] << endl;
	};
};

int main()
{
	using std::cin;
	using std::cout;
	using std::endl;

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

	Wine more("Gushing Grape Red",YRS, y, b);
	more.Show();
	cout << "Total bottles for " << more.Label() // используется метод Label()
		 << ": " << more.Sum() << endl; // используется метод Sum()
	cout << "Bye\n"; return 0;
}