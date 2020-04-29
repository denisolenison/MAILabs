#include<iostream>
#include<cmath>
#include<algorithm>
#include<vector>

using namespace std;

double function(double x) {
	return pow(2, x) + pow(x, 2) - 2; //=0
}

double der1(double x) {
	// (2^x + x^2 - 2)' = ln2 * 2^x + 2x
	return log(2) * pow(2, x) + 2 * x;
}

double der2(double x) {
	// (ln2 * 2^x + 2x)' = ln2 * ln2 * 2^x + 2
	return log(2) * log(2) * pow(2, x) + 2;
}


double phi(double x) {
	return sqrt(2 - pow(2, x)); //= x [0.6, 0.7]
		//x^2 = 2 - 2^x
		//x = sqrt(2 - 2^x)
}

double phi_der(double x) {
	return pow(2, x) * log(2) / (2 * sqrt(2 - pow(2, x)));
		//(sqrt(2 - 2^x))' = 2^x * ln2 / (2 * sqrt(2 - 2^x))
}



bool getEPS(double q, double newX, double prevX, double eps) {
	return ((q / (1 - q)) * abs(newX - prevX)) <= eps;
}



int main() {
	setlocale(LC_ALL, "Russian");

	//Метод простых итераций

	double eps;
	cout << "Введите точность E: ";
	cin >> eps;
	cout << "\n\nМетод простых итераций: \n\n";

	double a = 0.6;
	double b = 0.7;

	double q = abs(max(phi_der(a), phi_der(b)));

	double newX = (a + b) / 2;
	double prevX = -999999999;

	int iterations = 0;


	while (!getEPS(q, newX, prevX, eps)) {
		prevX = newX;
		newX = phi(prevX);
		iterations++;
	}

	cout << "Ответ: x = " << newX << "\n";
	cout << "Количество итераций: " << iterations << "\n";


	//Метод Ньютона

	cout << "\n\nМетод Ньютона: \n\n";

	if (der1(a) * der2(a) > 0) {
		newX = a;
	}
	else if (der1(b) * der2(b) > 0) {
		newX = b;
	}

	prevX = -999999999;

	iterations = 0;


	while (!(abs(newX - prevX) < eps)) {
		prevX = newX;
		newX = prevX - (function(prevX) / der1(prevX));
		++iterations;
	}

	cout << "Ответ: x = " << newX << "\n";
	cout << "Количество итераций: " << iterations << "\n\n";

	cout << "Вывод: Метод Ньютона куда проще и работает намного быстрее и точнее, требуется меньше итераций\n";

}