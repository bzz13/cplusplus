#include <iostream>
#include <cstring>
#include <new>
using namespace std;

struct chaff
{
	char dross[20];
	int slag;
};

char buffer1[100]{};
char * buffer2;

void fillchaff(chaff * chaff_ptr, const char * cstr, int slag)
{
	cout << "ptr addr: " << chaff_ptr << endl;
	strncpy(chaff_ptr->dross, cstr, 19);
	chaff_ptr->dross[20] = '\0';
	chaff_ptr->slag = slag;
}

void printBuffer(const char* buffer)
{
	cout << "buffer address: " << (void *) buffer << endl;
	for (int i = 0; i < 100; ++i)
	{
		if (buffer[i] == '\0')
			cout << ".";
		else
			if (buffer[i] > 'z')
				cout << int(buffer[i]);
			else
				cout << buffer[i];
	}
	cout << endl;
}

int main()
{
	buffer2 = new char[100]{};

	printBuffer(buffer1);
	printBuffer(buffer2);

	chaff * chaff_ptr1 = new(buffer1) chaff[2];
	cout << "chaff_ptr1 address: " << chaff_ptr1 << endl;

	chaff * chaff_ptr2 = new(buffer2) chaff[2];
	cout << "chaff_ptr2 address: " << chaff_ptr2 << endl;

	fillchaff(chaff_ptr1, "123qwe", 65);
	fillchaff(chaff_ptr1 + 1, "Name of structure", 80);

	fillchaff(chaff_ptr2, "123qwe", 65);
	fillchaff(chaff_ptr2 + 1, "Name of structure", 80);

	printBuffer(buffer1);
	printBuffer(buffer2);

	delete buffer2;
	return 0;
}