#include<iostream>
#include<cmath>
#include<algorithm>
#include<vector>

using namespace std;



double y(double x) {
	return 1 / (3 * pow(x, 2) + 4 * x + 2);
}








int main() {
	setlocale(LC_ALL, "Russian");
	int N;
	
	cout << "Вариант 7, Формула:\n\n";
	cout << "y = 1 / (3x^2 + 4x + 2)\n";

	double X0, Xk, h1, h2;
	cout << "\nВведите точку X0: ";
	cin >> X0;
	cout << "Введите точку Xk: ";
	cin >> Xk;
	cout << "Введите шаг h1: ";
	cin >> h1;
	cout << "Введите шаг h2: ";
	cin >> h2;


	double rect1 = 0;
	double rect2 = 0;

	double trap1 = 0;
	double trap2 = 0;

	double simpson1 = 0;
	double simpson2 = 0;
	int s_iter = 0;

	//решаем для h1
	double x = X0;

	trap1 += y(x) / 2 * h1;
	simpson1 += y(x) * h1 / 3;

	while (x + h1 <= Xk) {
		rect1 += y((2 * x + h1) / 2) * h1;

		if ((x + h1 != Xk)) {
			trap1 += y(x + h1) * h1;
		}
		else trap1 += y(x + h1) / 2 * h1;

		if ((x + h1 != Xk)) {
			if (s_iter % 2 == 0) {
				simpson1 += 4 * y(x + h1) * h1 / 3;
				s_iter++;
			}
			else {
				simpson1 += 2 * y(x + h1) * h1 / 3;
				s_iter++;
			}
		}
		else {
			simpson1 += y(x + h1) * h1 / 3;
		}
		x += h1;
	}



	//решаем для h2
	x = X0;

	trap2 += y(x) / 2 * h2;
	simpson2 += y(x) * h2 / 3;
	s_iter = 0;

	while (x + h2 <= Xk) {
		rect2 += y((2 * x + h2) / 2) * h2;

		if ((x + h2 != Xk)) {
			trap2 += y(x + h2) * h2;
		}
		else trap2 += y(x + h2) / 2 * h2;

		if ((x + h2 != Xk)) {
			if (s_iter % 2 == 0) {
				simpson2 += 4 * y(x + h2) * h2 / 3;
				s_iter++;
			}
			else {
				simpson2 += 2 * y(x + h2) * h2 / 3;
				s_iter++;
			}
		}
		else {
			simpson2 += y(x + h2) * h2 / 3;
		}
		x += h2;
	}


	//Погрешности и метод Рутте



	double k = h1 / h2;

	double rungeValue = 0;

	double exact = 1.857418687;


	double pogr_rect1 = abs(rect1 - rect2) / (pow(k, 2) - 1);
	double pogr_rect2 = abs(rect2 - exact) / (pow(k, 2) - 1);

	double pogr_trap1 = abs(trap1 - trap2) / (pow(k, 2) - 1);
	double pogr_trap2 = abs(trap2 - exact) / (pow(k, 2) - 1);

	double pogr_simp1 = abs(simpson1 - simpson2) / (pow(k, 4) - 1);
	double pogr_simp2 = abs(simpson2 - exact) / (pow(k, 4) - 1);
	





	cout << "\n";

	cout << "Значение интеграла методом прямоугольников с шагом h1 = " << h1 << " : " << rect1 << "\n";
	cout << "Значение интеграла методом прямоугольников с шагом h2 = " << h2 << " : " << rect2 << "\n\n";

	cout << "Значение интеграла методом трапеций с шагом h1 = " << h1 << " : " << trap1 << "\n";
	cout << "Значение интеграла методом трапеций с шагом h2 = " << h2 << " : " << trap2 << "\n\n";

	cout << "Значение интеграла методом Симпсона с шагом h1 = " << h1 << " : " << simpson1 << "\n";
	cout << "Значение интеграла методом Симпсона с шагом h2 = " << h2 << " : " << simpson2 << "\n\n";


	cout << "Погрешность зн. интеграла методом прямоугольников с шагом h1 = " << h1 << " : " << pogr_rect1 << "\n";
	cout << "Погрешность зн. интеграла методом прямоугольников с шагом h2 = " << h2 << " : " << pogr_rect2 << "\n\n";

	cout << "Погрешность зн. интеграла методом трапеций с шагом h1 = " << h1 << " : " << pogr_trap1 << "\n";
	cout << "Погрешность зн. интеграла методом трапеций с шагом h2 = " << h2 << " : " << pogr_trap2 << "\n\n";

	cout << "Погрешность зн. интеграла методом Симпсона с шагом h1 = " << h1 << " : " << pogr_simp1 << "\n";
	cout << "Погрешность зн. интеграла методом Симпсона с шагом h2 = " << h2 << " : " << pogr_simp2 << "\n\n";




}