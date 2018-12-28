#ifndef FIGURE_H
#define FIGURE_H
#include <memory>

class Figure
{
public:
	virtual size_t Square() = 0;
	virtual size_t GetA() = 0;
	virtual size_t GetB() = 0;
	virtual size_t GetC() = 0;
	virtual int GetFType() = 0;
	virtual void Print() = 0;
	virtual ~Figure() {};
};

#endif