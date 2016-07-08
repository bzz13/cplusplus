#include <iostream>
using namespace std;


int main()
{
	const auto inchCountInFeet = 12;
	const auto metrsInFeet = 0.0254;
	const auto poundInKg = 2.2;

	int heightFeets, heightInches, weightPounds;
	cout << "Введите ваш рост: " << endl << "футов:   \b\b";
	cin >> heightFeets;
	cout << "дюймов:   \b\b";
	cin >> heightInches;
	cout << "Введите ваш вес: фунтов:   \b\b";
	cin >> weightPounds;

	auto totalHeightMetrs = (heightInches + heightFeets * inchCountInFeet) * metrsInFeet;
	auto totalWieghtKg = weightPounds / poundInKg;

	cout << "BMI: " << totalWieghtKg / (totalHeightMetrs * totalHeightMetrs) << endl;
	return 0;
}