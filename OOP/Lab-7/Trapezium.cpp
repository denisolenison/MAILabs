#include "Trapezium.h"
#include <iostream>
#include <cmath>
#include <memory>

Trapezium::Trapezium() : Trapezium(0,0,0) {}

Trapezium::Trapezium(size_t i, size_t j, size_t k) : side_a(i), side_b(j), side_h(k)
{
	std::cout << "Trapezium created: " << side_a << ", " << side_b << ", " << side_h << std::endl;
}


Trapezium::Trapezium(const Trapezium& orig)
{
	std::cout << "Trapezium copy created" << std::endl;
	side_a = orig.side_a;
	side_b = orig.side_b;
	side_h = orig.side_h;
}

size_t Trapezium::Square()
{
	return size_t(double(side_a + side_b) * double(side_h) / 2);
}

void Trapezium::Print()
{
	std::cout << "(TR)[" << side_a << " | " << side_b << " | " << side_h << "]";
}

Trapezium& Trapezium::operator=(const Trapezium& right) {

	if (this == &right) return *this;

	std::cout << "Trapezium copied" << std::endl;
	side_a = right.side_a;
	side_b = right.side_b;
	side_h = right.side_h;

	return *this;
}

Trapezium& Trapezium::operator++() {

	side_a++;
	side_b++;
	side_h++;

	return *this;
}

int Trapezium::GetFType() {
	return 3;
}

size_t Trapezium::GetA() {
	return side_a;
}

size_t Trapezium::GetB() {
	return side_b;
}

size_t Trapezium::GetC() {
	return side_b;
}


Trapezium::~Trapezium()
{
	std::cout << "Trapezium deleted" << std::endl;
}


std::ostream& operator<<(std::ostream& os, const Trapezium& obj) {
	os << "<" << obj.side_a << " , " << obj.side_b << " , " << obj.side_h << ">";
	return os;
}

std::istream& operator>>(std::istream& is, Trapezium& obj) {

	is >> obj.side_a;
	is >> obj.side_b;
	is >> obj.side_h;

	return is;
}
