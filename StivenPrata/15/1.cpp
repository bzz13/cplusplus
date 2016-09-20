#include <iostream>
#include <exception>
#include <cstdlib>
using namespace std;

class Remote;

class Tv
{
	friend class Remote;

	bool isOn;
	int channel;
	int volume;
	const int maxChannel = 20;
	const int maxVolume = 20;

	void checkAnswer(const Remote& remote, const int & answer) const;
public:
	Tv(int c = 1, int v = 5): isOn(false), channel(c), volume(v) { };

	void turnOn() { isOn = true; }
	void turnOff() { isOn = false; }

	void nextChannel() { if (isOn) channel = channel == maxChannel ? channel + 1 : 1; else throw logic_error("tv is off"); }
	void prevChannel() { if (isOn) channel = channel == 1 ? maxChannel : 1; else throw logic_error("tv is off"); }

	void upVolume() { if (isOn) volume = volume == maxVolume ? maxVolume : volume + 1; else throw logic_error("tv is off"); }
	void downVolume() { if (isOn) volume = volume == 0 ? 0 : volume - 1; else throw logic_error("tv is off"); }

	void switchRemoteInteractive(Remote& remote) const;

	friend ostream & operator<<(ostream& os, const Tv & tv);
};

ostream & operator<<(ostream& os, const Tv & tv)
{
	if (tv.isOn)
		return (os << "Tv is on. Now channel: " << tv.channel << " volume: " << tv.volume << endl);
	else
		return (os << "Tv is off" << endl);
}

class Remote
{
	friend class Tv;

	bool isInteractive;
	void vibrate() const { cout << "\tbzzzzz"; }
public:
	Remote(): isInteractive(false) { };

	void turnOn(Tv& tv) { tv.isOn = true; }
	void turnOff(Tv& tv) { tv.isOn = false; }

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

	bool IsInteractive() const { return isInteractive; }
};

inline void Tv::checkAnswer(const Remote& remote, const int & answer) const
{
	int correctAnswer = rand() % 4;
	cout << " correct answer is " << correctAnswer;
	if (answer == correctAnswer)
		remote.vibrate();
}

inline void Tv::switchRemoteInteractive(Remote& remote) const
{
	remote.isInteractive = remote.isInteractive ? false : true;
}

int main()
{
	Tv tv;
	Remote remote;
	cout << tv << endl;

	remote.turnOn(tv);
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
	catch(logic_error& le)
	{
		cout << le.what() << endl;
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