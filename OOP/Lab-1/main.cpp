#include <cstdlib>
#include "Rectangle.h"
#include "Trapezium.h"
#include "Rhombus.h"

int main(int argc, char** argv)
{
	int k;
	std::cout << "Choose a figure:\n 1 - Rectangle\n 2 - Trapezium\n 3 - Rhombus" << std::endl;
	std::cin >> k;
	if (k == 1) // Rectangle
	{
		std::cout << "Enter sizes: " << std::endl;
		Figure *S = new Rectangle(std::cin);
		S->Print();
		std::cout << "Square = " << S->Square() << std::endl;
		delete S;
	}
	if (k == 2) // Trapezium
	{
		std::cout << "Enter 2 bases and height: " << std::endl;
		Figure *R = new Trapezium(std::cin);
		R->Print();
		std::cout << "Square = " << R->Square() << std::endl;
		delete R;
	}
	if (k == 3) // Rhombus
	{
		std::cout << "Enter heights: " << std::endl;
		Figure *T = new Rhombus(std::cin);
		T->Print();
		std::cout << "Square = " << T->Square() << std::endl;
		delete T;
	}
	system("PAUSE");
	return 0;
}