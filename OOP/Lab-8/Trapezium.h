#ifndef TRAPEZIUM_H
#define TRAPEZIUM_H
#include <cstdlib>
#include <iostream>
#include "Figure.h"
#include <memory>

class Trapezium : public Figure
{
public:
	Trapezium();
	Trapezium(size_t i, size_t j, size_t h);
	Trapezium(const Trapezium& orig);

	Trapezium& operator++();
	size_t Square() override;
	void Print() override;

	friend std::ostream& operator<<(std::ostream& os, const Trapezium& obj);
	friend std::istream& operator>>(std::istream& is, Trapezium& obj);

	Trapezium& operator=(const Trapezium& right);

	virtual ~Trapezium() override;
private:
	size_t side_a;
	size_t side_b;
	size_t side_h;
	int figureType = 3;
};

#endif