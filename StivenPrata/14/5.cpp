#include <iostream>
#include <string>
using namespace std;

class abstr_emp 
{
private:
	string fname; // HMHabstr_emp
	string lname; // фамилия abstr_emp
	string job;
public:
	abstr_emp(): fname("fname_not_set"), lname("lname_not_set"), job("job_not_set") { };
	abstr_emp(const string & fn, const string & ln, const string & j): fname(fn), lname(ln), job(j) { };
	virtual void ShowAll() const { cout << "fname: " << fname << ", lname: " << lname << ", job: " << job; }; // выводит все данные с именами 
	virtual void SetAll() { // запрашивает значения у пользователя
		cout << "fname: "; cin >> fname;
		cout << "lname: "; cin >> lname;
		cout << "job:   "; cin >> job;
	}; 
	friend ostream & operator<<(ostream & os, const abstr_emp & e) { // Выводит только имя и фамилию
		os << e.fname << " " << e.lname;
		return os;
	}; 
	virtual ~abstr_emp() = 0; // виртуальный базовый класс
};

abstr_emp::~abstr_emp() { }

class employee : public abstr_emp
{
public:
	employee(){};
	employee(const string & fn, const string & ln, const string & j): abstr_emp(fn, ln, j) { }; 
	virtual void ShowAll() const { abstr_emp::ShowAll(); }; 
	virtual void SetAll() { abstr_emp::SetAll(); };

	virtual ~employee(){ };
};

class manager: virtual public abstr_emp
{
private:
	int inchargeof; // количество управляемых abstr_emp
protected:
	int inChargeOf() const { return inchargeof; } // вывод
	int & inChargeOf () { return inchargeof; } // ввод
public:
	manager(){};
	manager(const string & fn, const string & ln, const string & j, int ico = 0): abstr_emp(fn, ln, j), inchargeof(ico) { };
	manager(const abstr_emp & e, int ico): abstr_emp(e), inchargeof(ico) { }; 
	manager(const manager & m): abstr_emp(m){
		inchargeof = m.inChargeOf();
	}; 
	virtual void ShowAll() const {  // выводит все данные с именами 
		abstr_emp::ShowAll();
		cout << ", inchargeof: " << inchargeof; 
	};
	virtual void SetAll() { // запрашивает значения у пользователя
		abstr_emp::SetAll();
		cout << "inchargeof: "; cin >> inchargeof;
	};
};

class fink: virtual public abstr_emp
{
private:
	string reportsto; // кому выводить отчеты
protected:
	const string ReportsTo () const { return reportsto; } 
	string & ReportsTo() { return reportsto; }
public:
	fink(){};
	fink(const string & fn, const string & ln, const string & j, const string & rpo): abstr_emp(fn, ln, j), reportsto(rpo) { };
	fink(const abstr_emp & e, const string & rpo): abstr_emp(e), reportsto(rpo) { };
	fink(const fink & f): abstr_emp(f) {
		reportsto = f.ReportsTo();
	}; 

	virtual void ShowAll() const {  // выводит все данные с именами 
		abstr_emp::ShowAll();
		cout << ", reportsto: " << reportsto; 
	};
	virtual void SetAll() { // запрашивает значения у пользователя
		abstr_emp::SetAll();
		cout << "reportsto: "; cin >> reportsto;
	};
};

class highfink: public manager, public fink // надзор за управляющими
{
public:
	highfink() {};
	highfink(const string & fn, const string & ln, const string & j, const string & rpo, int ico): abstr_emp(fn, ln, j), manager(fn, ln, j, ico), fink(fn, ln, j, rpo) { };
	highfink(const abstr_emp & e, const string & rpo, int ico): abstr_emp(e), manager(e, ico), fink(e, rpo) { };
	highfink(const fink & f, int ico): abstr_emp(f), manager(f, ico), fink(f) { };
	highfink(const manager & m, const string & rpo): abstr_emp(m), manager(m), fink(m, rpo) { };
	highfink(const highfink & h) ; 

	virtual void ShowAll() const {  // выводит все данные с именами 
		abstr_emp::ShowAll();
		cout << ", reportsto: " << fink::ReportsTo();
		cout << ", inchargeof: " << manager::inChargeOf();
	};
	virtual void SetAll() { // запрашивает значения у пользователя
		abstr_emp::SetAll();
		cout << "reportsto: "; cin >> fink::ReportsTo();
		cout << "inchargeof: "; cin >> manager::inChargeOf();
	};
};



int main(void)
{
	employee em("Trip", "Harris", "Thumper"); 
	cout << em << endl;
	em.ShowAll();
	cout << endl << endl;


	manager ma("Amorphia", "Spindragon", "Nuancer", 5);
	cout << ma << endl;
	ma.ShowAll();
	cout << endl << endl;

	fink fi("Matt", "Oggs", "Oiler", "Juno Barr");
	cout << fi << endl;
	fi.ShowAll() ;
	cout << endl << endl;


	highfink hf(ma, "Curly Kew"); // укомплектовано?
	hf.ShowAll();
	cout << hf << endl;
	cout << endl << endl;
	cout << "Press a key for next phase: \n" ;
	// Нажать любую клавишу для следующей фазы 
	cin.get(); 


	highfink hf2;
	hf2.SetAll();
	hf2.ShowAll();
	cout << endl << endl;


	cout << "Using an abstr_emp * pointer:\n";
	// Использование указателя abstr_emp * 

	abstr_emp * tri[4] = {&em, &fi, &hf, &hf2}; 
	for (int i = 0; i < 4; i++) 
	{
		tri[i]->ShowAll();
		cout << endl;
	}


	//answer
	// 1) достаточно почленного копирования
	// 2) различная реализация позволяет приводить к указателю на базовый класс и дергать методы
	// 3) есть чистый виртуальный метод(деструктор)
	// 4) из-за возможности приведения ссылки класса-наследника к ссылке класса-родителя
	// 5) compile error pure virtaul class abstr_emp

	return 0;
}