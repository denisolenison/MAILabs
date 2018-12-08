#ifndef RHOMBUS_H
#define RHOMBUS_H
#include <cstdlib>
#include <iostream>
#include "Figure.h"

class Rhombus : public Figure
{
public:
	Rhombus();
	Rhombus(std::istream &is);
	Rhombus(size_t i, size_t j);
	Rhombus(const Rhombus& orig);

	double Square() override;
	void Print() override;

	virtual ~Rhombus();
private:
	size_t side_a;
	size_t side_b;
};

#endif