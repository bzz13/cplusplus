#include "Cd2.h"

class ClassicCd: public Cd // представляет компакт-диск
{
private:
	char * mainTheme;
public:
	ClassicCd(const char * s1, const char * s2, const char * s3, int n, double x);
	ClassicCd();
	virtual ~ClassicCd();

	virtual void Report() const; // выводит все данные о компакт-диске

	ClassicCd & operator= (const ClassicCd & cd);
};