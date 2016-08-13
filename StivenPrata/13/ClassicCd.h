#include "Cd.h"

class ClassicCd: public Cd // представляет компакт-диск
{
private:
	char mainTheme[50];
public:
	ClassicCd(const char * s1, const char * s2, const char * s3, int n, double x);
	ClassicCd();

	virtual void Report() const; // выводит все данные о компакт-диске
	ClassicCd & operator= (const ClassicCd & cd);
};