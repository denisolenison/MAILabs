#include<iostream>
#include<cmath>
#include<algorithm>
#include<vector>

using namespace std;



int main() {
	setlocale(LC_ALL, "Russian");
	int N;
	cout << "Введите количество точек: ";
	cin >> N;
	vector<double>Xs(N, 0);
	vector<double>Ys(N, 0);
	double XStar;
	cout << "\nВведите точки X:\n";
	for (int i = 0; i < N; ++i) {
		cin >> Xs[i];
	}
	cout << "\nВведите точки Y:\n";
	for (int i = 0; i < N; ++i) {
		cin >> Ys[i];
	}
	cout << "\nВведите точку X*:\n";
	cin >> XStar;

	double firstDer;
	double secondDer;

	//Вычисление производных

	int num = -1;


	for (int i = 1; i < N-1; ++i) {
		if (XStar == Xs[i]) {
			num = i;
		}
	}

	if (num == -1) {
		cout << "\nУказаной точки нет в таблице, либо вычисление невозможно.\n";
		return -1;
	}

	double left1Der, right1Der;

	left1Der = (Ys[num] - Ys[num - 1]) / (Xs[num] - Xs[num - 1]);
	right1Der = (Ys[num + 1] - Ys[num]) / (Xs[num + 1] - Xs[num]);



	firstDer = left1Der + (right1Der - left1Der) / (Xs[num + 1] - Xs[num - 1]) * (2 * XStar - Xs[num - 1] - Xs[num]);

	secondDer = 2 * (right1Der - left1Der) / (Xs[num + 1] - Xs[num - 1]);


	cout << "\nОтвет:\n";
	cout << "y'(" << XStar << ") = " << firstDer << "\n";
	cout << "y''(" << XStar << ") = " << secondDer << "\n";

}