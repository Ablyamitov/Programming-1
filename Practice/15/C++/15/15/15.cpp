#include <iostream>
#include <ctime>
using namespace std;

int main()
{
	setlocale(LC_ALL, "Rus");
	link:
	srand(time(NULL));
	int n = rand() % 100;
	cout << "������������ � ����� ����. ���������� ������ ���������� �� 0 �� 100 �����\n";
	int b;
	for (int i = 0; i < 5; i++)
	{
		cin >> b;
		if (b == n) { cout << "����������! �� �������\n"; break; }
		if (i == 4) { cout << "�� ���������. ���� ��������: " << n << '\n'; break; }
		if (b < n) cout << "���������� ����� ������\n";
		if (b > n) cout << "���������� ����� ������\n";
	
	}
	int restart;
	cout << "������ ������ �������?(1 - ��)\n";
	cin >> restart;
	if (restart == 1) goto link;
}