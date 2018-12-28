#ifndef FIGURE_H
#define FIGURE_H
#include <memory>

class Figure
{
public:
	virtual size_t Square() = 0;
	virtual void Print() = 0;

	bool operator==(Figure& s) {
		return this->Square() == s.Square();
	}
	bool operator!=(Figure& s) {
		return this->Square() != s.Square();
	}
	bool operator<(Figure& s) {
		return this->Square() < s.Square();
	}
	bool operator>(Figure& s) {
		return this->Square() > s.Square();
	}
	bool operator<=(Figure& s) {
		return this->Square() <= s.Square();
	}
	bool operator>=(Figure& s) {
		return this->Square() >= s.Square();
	}

	operator double() {
		return this->Square();
	}

	virtual ~Figure() {};
};

#endif