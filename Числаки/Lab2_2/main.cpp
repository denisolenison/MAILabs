#include<iostream>
#include<cmath>
#include<algorithm>
#include<vector>

using namespace std;


const double E = 2.71828182846;


double f1(double x1, double x2) {
	return pow(x1, 2) + pow(x2, 2) - 4;
}


double f2(double x1, double x2) {
	return x1 - pow(E, x2) + 2;
}


double df1dx1(double x1, double x2) {
	return 2 * x1;
}

double df1dx2(double x1, double x2) {
	return 2 * x2;
}

double df2dx1(double x1, double x2) {
	return 1;
}

double df2dx2(double x1, double x2) {
	return -pow(E, x2);
}



double phi1(double x1, double x2) {
	return sqrt(4 - pow(x2, 2));
	//x1^2 = 4 - x2^2 //не сч отриц => x = sqrt(4 - x2^2)
}

double phi2(double x1, double x2) {
	return log(x1 + 2);
	//e^x2 = 2 + x1 => x2 = ln(x1 + 2)
}

double dphi1dx1(double x1, double x2) {
	return 0;
}

double dphi1dx2(double x1, double x2) {
	return -x2 / sqrt(4 - pow(x2, 2));
}

double dphi2dx1(double x1, double x2) {
	return 1 / (x1 + 2);
}

double dphi2dx2(double x1, double x2) {
	return 0;
}


double find_der(int i, int j, double x1, double x2) {
	if (i == 1) {
		if (j == 1) {
			return dphi1dx1(x1, x2);
		}
		else {
			return dphi1dx2(x1, x2);
		}
	}
	else {
		if (j == 1) {
			return dphi2dx1(x1, x2);
		}
		else {
			return dphi2dx2(x1, x2);
		}
	}
}


bool cond(double xPrev, double xNext, double eps, double q) {
	return (q / (1 - q)) * abs(xNext - xPrev) <= eps;
}


double detJ(double x1, double x2) {
	return df1dx1(x1, x2) * df2dx2(x1, x2) - df1dx2(x1, x2) * df2dx1(x1, x2);
}

double detA1(double x1, double x2) {
	return f1(x1, x2) * df2dx2(x1, x2) - df1dx2(x1, x2) * f2(x1, x2);
}

double detA2(double x1, double x2) {
	return df1dx1(x1, x2) * f2(x1, x2) - f1(x1, x2) * df2dx1(x1, x2);
}




int main() {
	setlocale(LC_ALL, "Russian");

	double eps;
	cout << "Введите точность E: ";
	cin >> eps;

	cout << "\nМетод итераций: \n\n";
	//Метод Итераций

	double p_x1 = 1.5;
	double p_x2 = 1.25;

	
	double q = max((find_der(1, 1, p_x1, p_x2) + find_der(1, 2, p_x1, p_x2)), (find_der(2, 1, p_x1, p_x2) + find_der(2, 2, p_x1, p_x2)));

	//cout << q;  q ~ 0.28 < 1 => все ОК!


	double x1Prev = -999999999;
	double x1Next = p_x1;

	double x2Prev = -999999999;
	double x2Next = p_x2;

	int iterations = 0;
	bool maxIter = false;

	while (!cond(x1Prev, x1Next, eps, q) || !cond(x2Prev, x2Next, eps, q)) {
		x1Prev = x1Next;
		x2Prev = x2Next;

		x1Next = phi1(x1Prev, x2Prev);
		x2Next = phi2(x1Prev, x2Prev);
		iterations++;
		if (iterations >= 5000) {
			maxIter = true;
			break;
		}
	}

	cout << "x1 = " << x1Next << "\n";
	cout << "x2 = " << x2Next << "\n";

	if (!maxIter) {
		cout << "Количество итераций: " << iterations << "\n";
	}
	else if (maxIter) {
		cout << "Количество итераций: " << iterations << " (Макс!)\n";
		cout << "Невозможно добиться соответствующей точности.\n";
	}

	//Метод Итераций

	cout << "\nМетод Ньютона: \n\n";

	x1Prev = -999999999;
	x1Next = p_x1;

	x2Prev = -999999999;
	x2Next = p_x2;

	iterations = 0;

	while (!(max((x1Next - x1Prev), (x2Next - x2Prev)) <= eps)) {
		x1Prev = x1Next;
		x2Prev = x2Next;

		x1Next = x1Prev - (detA1(x1Prev, x2Prev) / detJ(x1Prev, x2Prev));
		x2Next = x2Prev - (detA2(x1Prev, x2Prev) / detJ(x1Prev, x2Prev));
		iterations++;
	}

	cout << "x1 = " << x1Next << "\n";
	cout << "x2 = " << x2Next << "\n";
	cout << "Количество итераций: " << iterations << "\n";

}