#include "Rectangle.h"
#include <iostream>
#include <cmath>
#include <memory>

Rectangle::Rectangle() : Rectangle(0,0) {}

Rectangle::Rectangle(size_t i, size_t j) : side_a(i) , side_b(j)
{
	std::cout << "Rectangle created: " << side_a << ", " << side_b << std::endl;
}


Rectangle::Rectangle(const Rectangle& orig)
{
	std::cout << "Rectangle copy created" << std::endl;
	side_a = orig.side_a;
	side_b = orig.side_b;
}

double Rectangle::Square()
{
	return double(side_a) * double(side_b);
}


void Rectangle::Print()
{
	std::cout << "(REC)[" << side_a << " , " << side_b << "]" << " - ";
}



Rectangle& Rectangle::operator=(const Rectangle& right) {

	if (this == &right) return *this;

	std::cout << "Rectangle copied" << std::endl;
	side_a = right.side_a;
	side_b = right.side_b;

	return *this;
}

Rectangle& Rectangle::operator++() {

	side_a++;
	side_b++;

	return *this;
}

Rectangle::~Rectangle()
{
	std::cout << "Rectangle deleted" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Rectangle& obj) {
	os << "<" << obj.side_a << " , " << obj.side_b << ">";
	return os;
}

std::istream& operator>>(std::istream& is, Rectangle& obj) {

	is >> obj.side_a;
	is >> obj.side_b;

	return is;
}
