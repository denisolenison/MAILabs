#include "Rhombus.h"
#include <iostream>
#include <cmath>
#include <memory>

Rhombus::Rhombus() : Rhombus(0,0) {}

Rhombus::Rhombus(size_t i, size_t j) : side_a(i), side_b(j)
{
	std::cout << "Rhombus created: " << side_a << ", " << side_b << std::endl;
}


Rhombus::Rhombus(const Rhombus& orig)
{
	std::cout << "Rhombus copy created" << std::endl;
	side_a = orig.side_a;
	side_b = orig.side_b;
}

size_t Rhombus::Square()
{
	return size_t(double(side_a) * double(side_b) / 2);
}

void Rhombus::Print()
{
	std::cout << "(RH)[" << side_a << " | " << side_b << "]";
}


Rhombus& Rhombus::operator=(const Rhombus& right) {

	if (this == &right) return *this;

	std::cout << "Rhombus copied" << std::endl;
	side_a = right.side_a;
	side_b = right.side_b;

	return *this;
}

Rhombus& Rhombus::operator++() {

	side_a++;
	side_b++;

	return *this;
}

int Rhombus::GetFType() {
	return 2;
}

size_t Rhombus::GetA() {
	return side_a;
}

size_t Rhombus::GetB() {
	return side_b;
}

size_t Rhombus::GetC() {
	return 0;
}

Rhombus::~Rhombus()
{
	std::cout << "Rhombus deleted" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Rhombus& obj) {
	os << "<" << obj.side_a << " , " << obj.side_b << ">";
	return os;
}

std::istream& operator>>(std::istream& is, Rhombus& obj) {

	is >> obj.side_a;
	is >> obj.side_b;

	return is;
}
