#include "imag.h"
#include<algorithm>


Imag::Imag(double real, double im) : real(real), im(im) {
}
Imag::Imag(double real) : real(real), im(0) {
}
Imag::Imag() : real(0), im(0) {
}


Imag Imag::sqrt(Imag val) {
		if (val.real != 0 || val.im != 0) {
			double realPart = val.real;
			double imPart = val.im;

			double maxAbs = std::sqrt((realPart * realPart) + (imPart * imPart));

			double phi;


			if (realPart != 0) {
				double tanPart = imPart / realPart;


				if (imPart > 0) {
					phi = std::atan(tanPart);
				}
				else if (imPart < 0) {
					phi = pi + std::atan(tanPart);
				}
				else if (realPart < 0 && imPart == 0) {
					phi = pi;
				}
				else {
					phi = 0;
				}
			}
			else {
				if (imPart > 0) {
					phi = pi / 2;
				}
				else {
					phi = -pi / 2;
				}
			}


			double realSq = std::sqrt(std::sqrt(realPart*realPart + imPart*imPart));

			double cosPart = std::cos(phi / 2);
			double sinPart = std::sin(phi / 2);

			realPart = realSq * cosPart;
			imPart = realSq * sinPart;

			if (realPart < maxAprox) {
				realPart = 0;
			}

			if (imPart < maxAprox) {
				imPart = 0;
			}


			Imag newIm = Imag(realPart, imPart);

			return newIm;
		}
		else {
			return 0;

		}

	}