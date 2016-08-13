class Cd // представляет компакт-диск
{
private:
	char * performers;
	char * label;
	int selections; // количество сборников
	double playtime; // время воспроизведения в минутах
public:
	Cd(const char * s1, const char * s2, int n, double x);
	Cd(const Cd & d);
	Cd();
	virtual ~Cd();

	virtual void Report() const; // выводит все данные о компакт-диске

	Cd & operator= (const Cd & cd);
};