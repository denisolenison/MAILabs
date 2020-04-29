#ifndef TDROB_H
#define TDROB_H

#include <iostream>
#include <cmath>
#include <math.h>
#include <algorithm>

const double pi = ::std::atan(1) * 4;

const double maxAprox = 1e-12;

class Imag {
private:
	double real;
	double im;
public:
	Imag(double real, double im);
	Imag(double real);
	Imag();

	friend Imag operator+ (Imag c) {
		return Imag(c.real, c.im);
	}
	friend Imag operator- (Imag c) {
		return Imag(-c.real, -c.im);
	}
	friend Imag operator+ (Imag c1, Imag c2) {
		double realPart = c1.real + c2.real;
		double imPart = c1.im + c2.im;
		Imag NewImag = Imag(realPart, imPart);
		return NewImag;
	}
	friend Imag operator- (Imag c1, Imag c2) {
		return c1 + (-c2);
	}
	friend Imag operator* (Imag c1, Imag c2) {
		double realPart = (c1.real * c2.real) - (c1.im * c2.im);
		double imPart = (c1.real * c2.im) + (c2.real * c1.im);
		Imag NewImag = Imag(realPart, imPart);
		return NewImag;
	}
	friend Imag operator/ (Imag c1, double div) {
		Imag NewImag = Imag(c1.real / div, c1.im / div);
		return NewImag;
	}
	friend Imag operator/ (Imag c1, Imag c2) {
		Imag c2New = Imag(c2.real, -c2.im);
		Imag c2Tot = c2 * c2New;
		double divider = c2Tot.real;

		return (c1 * c2New) / divider;
	}

	const Imag& operator+= (const Imag& right) {
		*this = *this + right;
		return *this;
	}

	const Imag& operator-= (const Imag& right) {
		*this = *this - right;
		return *this;
	}

	const Imag& operator+= (const double& right) {
		*this = *this + Imag(right);
		return *this;
	}

	const Imag& operator-= (const double& right) {
		*this = *this - Imag(right);
		return *this;
	}

	const Imag& operator*= (const Imag& right) {
		*this = *this * right;
		return *this;
	}

	const Imag& operator/= (const Imag& right) {
		*this = *this / right;
		return *this;
	}

	const Imag& operator*= (const double& right) {
		*this = *this * Imag(right);
		return *this;
	}

	const Imag& operator/= (const double& right) {
		*this = *this / right;
		return *this;
	}

	Imag& operator++ () {
		*this += 1;
		return *this;
	}

	Imag& operator-- () {
		*this -= 1;
		return *this;
	}

	friend bool operator== (const Imag& left, const Imag& right) {
		if (left.real == right.real && left.im == right.im) {
			return true;
		}
		else return false;
	}

	friend bool operator== (const Imag& left, const double& right) {
		if (left.real == right && left.im == 0) {
			return true;
		}
		else return false;
	}

	friend bool operator== (const double& left, const Imag& right) {
		if (left == right.real && right.im == 0) {
			return true;
		}
		else return false;
	}

	friend bool operator!= (const Imag& left, const Imag& right) {
		return !(left == right);
	}

	friend bool operator> (const Imag& left, const Imag& right) {
		return (left.real * right.real);
	}

	friend bool operator< (const Imag& left, const Imag& right) {
		return right > left;
	}

	friend bool operator>= (const Imag& left, const Imag& right) {
		return (left > right) || (left == right);
	}

	friend bool operator<= (const Imag& left, const Imag& right) {
		return (left < right) || (left == right);
	}



	static Imag sqrt(Imag val);


	friend std::istream& operator>> (std::istream& is, Imag& num) {
		is >> num.real;
		num.im = 0;
		return is;
	}

	friend std::ostream& operator<< (std::ostream& out, const Imag& num) {
		if (num.real != 0 && num.im != 0) {
			if (num.im != 1) {
				out << num.real << " + " << num.im << "i";
			}
			else {
				out << num.real << " + i";
			}
		}
		else if (num.im != 0) {
			if (num.im != 1) {
				out << num.im << "i";
			}
			else {
				out << "i";
			}
		}
		else if (num.real != 0) {
			out << num.real;
		}
		else {
			out << 0;
		}
		return out;
	}

	~Imag() {};

};


#endif