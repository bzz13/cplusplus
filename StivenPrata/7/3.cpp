#include <iostream>
using namespace std;

struct box {
	char maker[40];
	float height;
	float width;
	float length;
	float volume;
};

void print_box(const box);
void calc_volume_of_box(box *);

int main()
{
	box b{"test box", 10, 20, 30};
	cout << "before calc volume" << endl;
	print_box(b);
	calc_volume_of_box(&b);
	cout << endl << "after calc volume" << endl;
	print_box(b);
}

void print_box(const box b)
{
	cout << "Box: { "
		 << "m =\"" << b.maker << "\", "
		 << "h =\"" << b.height << "\", "
		 << "w =\"" << b.width << "\", "
		 << "l =\"" << b.length << "\", "
		 << "v =\"" << b.volume << "\"}"
		 << endl;
}
void calc_volume_of_box(box * b)
{
	b->volume = b->height * b->width * b->length;
}