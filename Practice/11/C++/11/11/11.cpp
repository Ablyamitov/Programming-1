#include <iostream>
using namespace std;

int main()
{
	setlocale(LC_ALL, "Rus");
	double a, c;
	int b;
	cout << "������� �����, ������� ����� �������� � �������: ";
	cin >> a;
	cout << "������� �������, � ������� ����� �������� �����: ";
	cin >> b;
	c = a;
	
	if (b > 0)
	{ for (int i = 1; i < b; i++) a = a * c; }
	else if (b == 0) a = 1;
	else
	{
		for (; b < -1; b++) { a = a * c; }
		a = 1 / a;
	}

	cout << "���������: " << a;

}