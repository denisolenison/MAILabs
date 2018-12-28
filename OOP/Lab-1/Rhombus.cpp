#include "Rhombus.h"
#include <iostream>
#include <cmath>

Rhombus::Rhombus() : Rhombus(0,0) {}

Rhombus::Rhombus(size_t i, size_t j) : side_a(i), side_b(j)
{
	std::cout << "Rhombus created: " << side_a << ", " << side_b << std::endl;
}

Rhombus::Rhombus(std::istream &is)
{
	is >> side_a;
	is >> side_b;
}

Rhombus::Rhombus(const Rhombus& orig)
{
	std::cout << "Rhombus copy created" << std::endl;
	side_a = orig.side_a;
	side_b = orig.side_b;
}

double Rhombus::Square()
{
	return side_a * side_b / 2;
}

void Rhombus::Print()
{
	std::cout << "h1 = " << side_a << ", h2 = " << side_b << std::endl;
}

Rhombus::~Rhombus()
{
	std::cout << "Rhombus deleted" << std::endl;
}