#ifndef TRAPEZIUM_H
#define TRAPEZIUM_H
#include <cstdlib>
#include <iostream>
#include "Figure.h"

class Trapezium : public Figure
{
public:
	Trapezium();
	Trapezium(std::istream &is);
	Trapezium(size_t i, size_t j, size_t h);
	Trapezium(const Trapezium& orig);

	double Square() override;
	void Print() override;

	virtual ~Trapezium();
private:
	size_t side_a;
	size_t side_b;
	size_t side_h;
};

#endif