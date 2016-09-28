#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <vector>
#include <algorithm>

using namespace std;

class reader
{
	ifstream is;
	istream_iterator<string> eos;
	istream_iterator<string> it;
public:
	reader(const string& in)
	{
		is.open(in);
		it = istream_iterator<string>(is);
	}
	~reader()
	{
		is.close();
	}
	vector<string> readToEnd()
	{
		vector<string> result;
		while(it != eos)
		{
			result.push_back(*it);
			++it;
		}
		return result;
	}
};

int main()
{
	srand(time(0));
	char play;
	cout << "Will you play a word game? <y/n> "; // запуск игры в слова
	cin >> play;
	play = tolower(play);
	while (play == 'y')
	{
		reader rd("3in.txt");
		vector<string> wordlist(rd.readToEnd());

		string target = wordlist[rand() % wordlist.size()];
		cout << target << endl;
		int length = target.length();
		string attempt(length, '-');
		string badchars;
		int guesses = 6;
		cout << "Guess my secret word. It has " << length << " letters, and you guess" << endl
		     << "one letter at a time. You get " << guesses << " wrong guesses." << endl;
		cout << "Your word: " << attempt << endl; // вывод слова
		while (guesses > 0 && attempt != target)
		{
			char letter;
			cout << "Guess a letter: ";
			cin >> letter;
			if (badchars.find(letter) != string::npos
			        || attempt.find(letter) != string::npos)
			{
				cout << "You already guessed that. Try again." << endl;
				continue;
			}
			int loc = target.find(letter);
			if (loc == string::npos)
			{
				cout << "Oh, bad guess !" << endl;
				--guesses;
				badchars += letter; // добавить к строке
			}
			else
			{
				cout << "Good guess!" << endl; attempt[loc] = letter; // Проверить, не появляется ли буква еще раз
				loc = target. find (letter, loc + 1) ;
				while (loc != string::npos)
				{
					attempt[loc] = letter;
					loc = target. find (letter, loc + 1);
				}
				cout << "Your word: " << attempt << endl;
				if (attempt != target && badchars.length() > 0)
				{
					cout << "Bad choices: " << badchars << endl;
				}
			}
			cout << guesses << " bad guesses left" << endl;
		}
		if (guesses > 0)
			cout << "That's right!" << endl;
		else
			cout << "Sorry, the word is " << target << "." << endl;
		cout << "Will you play another? <y/n> ";
		cin >> play;
		play = tolower(play);
	}
	cout << "Bye" << endl;
	return 0;
}