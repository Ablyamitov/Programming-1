#include <iostream>
using namespace std;

int main()
{ 
	setlocale(LC_ALL, "Rus");
{
	int a, b;
	cout << "������� �������� ��������� a: ";
	cin >> a;
	cout<< "������� �������� ��������� b: ";
	cin >> b;
	cout << "a + b = " << a + b << endl << "a - b = " << a - b << endl << "a * b = " << a * b << endl << "a / b = " << a / b << endl;
}
{
	double a, b;
	cout << "������� �������� ��������� a: ";
	cin >> a;
	cout << "������� �������� ��������� b: ";
	cin >> b;
	cout << "a + b = " << a + b << endl << "a - b = " << a - b << endl << "a * b = " << a * b << endl << "a / b = " << a / b << endl;
}
{
	int a;
	double b;
	cout << "������� �������� ��������� a: ";
	cin >> a;
	cout << "������� �������� ��������� b: ";
	cin >> b;
	cout << "a + b = " << a + b << endl << "a - b = " << a - b << endl << "a * b = " << a * b << endl << "a / b = " << a / b << endl;
}
{
	double a;
	int b;
	cout << "������� �������� ��������� a: ";
	cin >> a;
	cout << "������� �������� ��������� b: ";
	cin >> b;
	cout << "a + b = " << a + b << endl << "a - b = " << a - b << endl << "a * b = " << a * b << endl << "a / b = " << a / b << endl;
}


}