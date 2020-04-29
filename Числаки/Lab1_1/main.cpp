#include<iostream>
#include<vector>
#include "TDrob.h"
#include "WorkMatrix.h"

using namespace std;


int main() {
	setlocale(LC_ALL, "Russian");
	cout << "¬ведите размер матрицы\n";
	int N;
	cin >> N;
	vector<vector<TDrob> >matrix(N, vector<TDrob>(N, 0));
	vector<vector<TDrob> >matrixL(N, vector<TDrob>(N, 0));
	vector<TDrob>solve(N, 0);
	vector<TDrob>answerY(N, 0);
	vector<TDrob>answer(N, 0);


	vector<vector<TDrob> >matrixUObr(N, vector<TDrob>(N, 0));
	vector<vector<TDrob> >matrixLObr(N, vector<TDrob>(N, 0));
	vector<vector<TDrob> >matrixObr(N, vector<TDrob>(N, 0));

	TDrob det = 1;

	cout << "¬ведите матрицу A\n";
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			cin >> matrix[i][j];
		}
	}
	cout << "¬ведите вектор B\n";
	for (int i = 0; i < N; ++i) {
		cin >> solve[i];
	}

	for (int i = 0; i < N; ++i) {
		matrixL[i][i] = 1;
	}

	for (int r = 0; r < N - 1; ++r) {
		for (int i = r+1; i < N; ++i) {
			TDrob koef = matrix[i][r] / matrix[r][r];
			matrixL[i][r] = koef;
			for (int j = 0; j < N; ++j) {
				matrix[i][j] -= matrix[r][j] * koef;
			}
			//solve[i] -= solve[r] * koef;
		}
	}

	answerY[0] = solve[0];
	for (int i = 1; i < N; ++i) {
		TDrob value = 0;
		for (int j = 0; j < i; ++j) {
			value += answerY[j] * matrixL[i][j];
		}
		answerY[i] = solve[i] - value;
	}


	answer[N - 1] = answerY[N - 1] / matrix[N - 1][N - 1];
	for (int i = N - 2; i >= 0; --i) {
		TDrob value = 0;
		for (int j = N - 1; j > i; --j) {
			value += answer[j] * matrix[i][j];
		}
		answer[i] = (answerY[i] - value) / matrix[i][i];
	}


	for (int i = 0; i < N; ++i) {
		det *= matrix[i][i];
	}


	for (int i = 0; i < N; ++i) {
		matrixLObr[i][i] = 1 / matrixL[i][i];
		matrixUObr[i][i] = 1 / matrix[i][i];
	}

	for (int i = 1; i < N; ++i) {
		for (int j = 0; j < i; ++j) {
			TDrob value = 0;
			for (int k = j; k < i; ++k) {
				value -= matrixL[i][k] * matrixLObr[k][j];
			}
			matrixLObr[i][j] = value / matrixL[i][i];
		}
	}


	for (int i = N - 2; i >= 0; --i) {
		for (int j = N - 1; j > i; --j) {
			TDrob value = 0;
			for (int k = j; k > i; --k) {
				value -= matrix[i][k] * matrixUObr[k][j];
			}
			matrixUObr[i][j] = value / matrix[i][i];
		}
	}



	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			for (int k = 0; k < N; ++k) {
				matrixObr[i][j] += matrixUObr[i][k] * matrixLObr[k][j];
			}
		}
	}






	cout << "ќтвет:\n";
	for (int i = 0; i < N; ++i) {
		cout << answer[i] << "\n";
	}
	cout << "ќпределитель:\n";
	cout << det << "\n";

	cout << "ќбратна€ матрица:\n";
	PrintMatrix(matrixObr);



}