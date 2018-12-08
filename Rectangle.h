#ifndef RECTANGLE_H
#define RECTANGLE_H
#include <cstdlib>
#include <iostream>
#include "Figure.h"

class Rectangle : public Figure
{
public:
	Rectangle();
	Rectangle(std::istream &is);
	Rectangle(size_t i, size_t j);
	Rectangle(const Rectangle& orig);

	double Square() override;
	void Print() override;

	virtual ~Rectangle();
private:
	size_t side_a;
	size_t side_b;
};

#endif