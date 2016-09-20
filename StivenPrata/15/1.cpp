#include <iostream>
#include <exception>
#include <cstdlib>
using namespace std;

class Remote;

class Tv
{
	friend class Remote;

	int channel;
	int volume;
	const int maxChannel = 20;
	const int maxVolume = 20;

	void checkAnswer(const Remote& remote, const int & answer) const;
public:
	Tv(int c = 1, int v = 5): channel(c), volume(v) { };

	void nextChannel() { channel = channel == maxChannel ? channel + 1 : 1; }
	void prevChannel() { channel = channel == 1 ? maxChannel : 1; }

	void upVolume() { volume = volume == maxVolume ? maxVolume : volume + 1; }
	void downVolume() { volume = volume == 0 ? 0 : volume - 1; }

	friend ostream & operator<<(ostream& os, const Tv & tv);
};

ostream & operator<<(ostream& os, const Tv & tv)
{
	return (os << "channel: " << tv.channel << " volume: " << tv.volume << endl);
}

class Remote
{
	friend class Tv;

	void vibrate() const { cout << "\tbzzzzz"; }
public:
	Remote() { };

	void setChannel(Tv& tv, int c)
	{
		if (c > 1 && c < tv.maxVolume)
			tv.channel = c;
		else
			throw invalid_argument("incorrect channel");
	}

	void nextChannel(Tv& tv) { tv.nextChannel(); }
	void prevChannel(Tv& tv) { tv.prevChannel(); }

	void upVolume(Tv& tv) { tv.upVolume(); }
	void downChannel(Tv& tv) { tv.downVolume(); }

	void setAnswer(const Tv& tv, const int & answer) { tv.checkAnswer(*this, answer); }
};

inline void Tv::checkAnswer(const Remote& remote, const int & answer) const
{
	int correctAnswer = rand() % 4;
	cout << " correct answer is " << correctAnswer;
	if (answer == correctAnswer)
		remote.vibrate();
}

int main()
{
	Tv tv;
	Remote remote;
	cout << tv << endl;

	remote.setChannel(tv, 15);
	remote.upVolume(tv);
	cout << tv << endl;

	try
	{
		remote.setChannel(tv, 25);
		cout << tv << endl;
	}
	catch(invalid_argument& ia)
	{
		cout << ia.what() << endl;
	}

	for (int i = 0; i < 10; ++i)
	{
		int answer = rand() % 4;
		cout << "check answer " << answer << ":";
		remote.setAnswer(tv, answer);
		cout << endl;
	}

	return 0;
}