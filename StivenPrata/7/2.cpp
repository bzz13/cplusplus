#include <iostream>
using namespace std;

const int Limit = 10;

int getScores(int *, int);
void showScores(const int *, int);
double getMedian(const int *, int);

int main()
{
	int scores[Limit];
	int count = getScores(scores, Limit);

	showScores(scores, count);
	cout << getMedian(scores, count);

	return 0;
}

int getScores(int * arr, int count)
{
	int readed = 0;
	for (int i = 0; i < count; ++i, ++readed)
	{
		cout << "enter score of " << (i+1) << " game: ";
		if (!(cin >> arr[i]))
			break;
	}
	return readed;
}
void showScores(const int * arr, int count)
{
	cout << "Scores: ";
	for (int i = 0; i < count; ++i)
		cout << arr[i] << " ";
	cout << endl;
}
double getMedian(const int * arr, int count)
{
	double result = 0;
	for (int i = 0; i < count; ++i)
		result += arr[i];
	result /= count;
	return result;
}