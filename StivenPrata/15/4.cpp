#include "sales.h"

int main()
{
	double vals1[12] =
	{
		1220, 1100, 1122, 2212, 1232, 2334,
		2884, 2393, 3302, 2922, 3002, 3544
	};
	double vals2[12] =
	{
		12, 11, 22, 21, 32, 34,
		28, 29, 33, 29, 32, 35
	};

	Sales sales1(2011, vals1, 12);
	LabeledSales sales2("Blogstar", 2012, vals2, 12 );
	cout << "First try block: " << endl; // первый блок try
	try
	{
		int i;
		cout << "Year = " << sales1.Year() << endl; // год
		for (i = 0; i < 12; ++i)
		{
			cout << sales1 [i] << ' '; if (i % 6 == 5) cout << endl;
		}
		cout << "Year = " << sales2.Year() << endl; // год
		cout << "Label = " << sales2.Label() << endl; // метка
		for (i = 0; i <= 12; ++i)
		{
			cout << sales2[i] << ' '; if (i % 6 == 5) cout << endl;
		}
		cout << "End of try block 1." << endl; // конец первого блока try
	}
	catch (Sales::bad_index & bad)
	{
		cout << bad.what();
		cout << "bad_index: " << bad.bi_val() << endl; // недопустимый индекс

		LabeledSales::nbad_index * ndb;
		if (ndb = dynamic_cast<LabeledSales::nbad_index *>(&bad))
			cout << "Company: " << ndb->label_val() << endl; // компания
	}

	cout << endl << "Next try block: " << endl; // второй блок try
	try
	{
		sales2 [ 2] = 37.5; sales1[20] = 23345;
		cout << "End of try block 2." << endl; // конец второго блока try
	}
	catch (Sales::bad_index & bad)
	{
		cout << bad.what();
		cout << "bad_index: " << bad.bi_val() << endl;
		LabeledSales::nbad_index * ndb;
		if (ndb = dynamic_cast<LabeledSales::nbad_index *>(&bad))
			cout << "Company: " << ndb->label_val() << endl; // компания

		cout << "done" << endl;
	}

	return 0;
}