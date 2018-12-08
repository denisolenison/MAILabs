#include "Trapezium.h"
#include <iostream>
#include <cmath>

Trapezium::Trapezium() : Trapezium(0,0,0) {}

Trapezium::Trapezium(size_t i, size_t j, size_t k) : side_a(i), side_b(j), side_h(k)
{
	std::cout << "Trapezium created: " << side_a << ", " << side_b << ", " << side_h << std::endl;
}

Trapezium::Trapezium(std::istream &is)
{
	is >> side_a;
	is >> side_b;
	is >> side_h;

}

Trapezium::Trapezium(const Trapezium& orig)
{
	std::cout << "Trapezium copy created" << std::endl;
	side_a = orig.side_a;
	side_b = orig.side_b;
	side_h = orig.side_h;
}

double Trapezium::Square()
{
	return (side_a + side_b) * side_h / 2;
}

void Trapezium::Print()
{
	std::cout << "a = " << side_a << ", b = " << side_b << ", h = " << side_h << std::endl;
}

Trapezium::~Trapezium()
{
	std::cout << "Trapezium deleted" << std::endl;
}