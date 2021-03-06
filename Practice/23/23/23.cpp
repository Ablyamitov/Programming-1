﻿#include <iostream>
#include <iomanip>
#include "fact.h"
#include "sin.h"
#include "sochetaniya.h"

const double PI = 3.141592653589793;
int main()
{
	std::cout << "n\tn!" << std::endl;
	for (int i = 1; i <= 10; i++)
		std::cout << i << '\t' << fact(i) << std::endl;
	std::cout << std::endl;
	std::cout << "x\tsin(x)" << std::endl;
	for (double x = 0; x <= PI / 4; x += PI / 180)
	{
		std::cout << std::setprecision(4);
		std::cout << x << '\t' << sin(x, 5) << std::endl;
	}
	std::cout << std::endl;

	std::cout << "k\tC(k, 10)" << std::endl;
	for (int k = 1; k <= 10; k++)
	{
		std::cout << k << '\t' << sochetaniya(k, 10) << std::endl;
	}	
}