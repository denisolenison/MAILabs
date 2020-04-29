#include<iostream>
#include<vector>
#include "TDrob.h"
#include "WorkMatrix.h"


const int N = 5;

using namespace std;


int main() {
	setlocale(LC_ALL, "Russian");
	cout << "Введите матрицу 5х5\n";

	vector<vector<TDrob> >matrix(N, vector<TDrob>(N, 0));

	vector<TDrob>vectorB(N, 0);
	vector<TDrob>vectorX(N, 0);


	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			cin >> matrix[i][j];
		}
	}

	cout << "Введите вектор B\n";

	for (int i = 0; i < N; ++i) {
		cin >> vectorB[i];
	}


	vector<TDrob>alphas(N, 0);
	vector<TDrob>betas(N, 0);

	//Прямой ход прогонки

	for (int i = 0; i < N; ++i) {
		TDrob A0, C0, B0, F0;

		if (i - 1 < 0) {
			A0 = 0;
		}
		else {
			A0 = matrix[i][i - 1];
		}

		C0 = -1 * matrix[i][i];

		if (i + 1 < N) {
			B0 = matrix[i][i + 1];
		}
		else {
			B0 = 0;
		}

		F0 = vectorB[i];


		if (i == 0) {
			alphas[i] = B0 / C0;
			betas[i] = -(F0 / C0);
		}
		else if (i == N - 1) {
			alphas[i] = 0;
			betas[i] = (betas[i - 1] * A0 - F0) / (C0 - alphas[i - 1] * A0);
		}
		else {
			alphas[i] = B0 / (C0 - alphas[i - 1] * A0);
			betas[i] = (betas[i - 1] * A0 - F0) / (C0 - alphas[i - 1] * A0);
		}

	}


	vectorX[N - 1] = betas[N - 1];

	for (int i = 2; i <= N; ++i) {
		vectorX[N - i] = alphas[N - i] * vectorX[N - i + 1] + betas[N - i];
	}

	cout << "Ответ:\n";

	for (int i = 0; i < N; ++i) {
		cout << "x" << (i + 1) << " = " << vectorX[i] << "\n";
	}


}