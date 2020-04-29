#include<iostream>
#include<vector>
#include "imag.h"
#include <iomanip>


const int N = 3;

using namespace std;



double signF(double a) {
	if (a < 0) {
		return -1;
	}
	else {
		return 1;
	}
}


const double minNum = 1e-12;



void getQR(vector<vector<double> > matrixA, vector<vector<double> >& matrQ, vector<vector<double> >& matrR) {


	vector<vector<double> >matrixE(N, vector<double>(N, 0));

	vector<vector<double> >matrixCur(N, vector<double>(N, 0));


	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			if (i != j) {
				matrQ[i][j] = 0;
			}
		}
	}

	for (int i = 0; i < N; ++i) {
		matrixE[i][i] = 1;
		matrQ[i][i] = 1;
	}

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			matrixCur[i][j] = matrixA[i][j];
		}
	}

	for (int i = 0; i < N - 1; ++i) {
		vector<double> vectorV(N, 0);

		for (int j = 0; j < N; ++j) {
			if (j < i) {
				vectorV[j] = 0;
			}
			else if (j == i) {
				double sumGet = 0;
				for (int k = j; k < N; ++k) {
					sumGet += matrixCur[k][j] * matrixCur[k][j];
				}
				vectorV[j] = matrixCur[i][i] + signF(matrixCur[i][i]) * sqrt(sumGet);
			}
			else {
				vectorV[j] = matrixCur[j][i];
			}
		}

		double vectorNum = 0;
		vector<vector<double> > vectorMatr(N, vector<double>(N, 0));

		vector<vector<double> > MatrH(N, vector<double>(N, 0));

		vector<vector<double> > MatrA(N, vector<double>(N, 0));

		for (int i = 0; i < N; ++i) {
			vectorNum += vectorV[i] * vectorV[i];
		}

		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				vectorMatr[i][j] = vectorV[i] * vectorV[j];
			}
		}

		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				MatrH[i][j] = matrixE[i][j] - (2 * vectorMatr[i][j] / vectorNum);
			}
		}

		vector<vector<double> > MatrRes(N, vector<double>(N, 0));

		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				for (int k = 0; k < N; ++k) {
					MatrRes[i][j] += matrQ[i][k] * MatrH[k][j];
				}
			}
		}


		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				if (abs(MatrRes[i][j]) > minNum) {
					matrQ[i][j] = MatrRes[i][j];
				}
				else {
					matrQ[i][j] = 0;
				}
			}
		}


		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				for (int k = 0; k < N; ++k) {
					MatrA[i][j] += MatrH[i][k] * matrixCur[k][j];
				}
			}
		}

		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				matrixCur[i][j] = MatrA[i][j];
			}
		}

	}


	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			if (abs(matrixCur[i][j]) > minNum) {
				matrR[i][j] = matrixCur[i][j];
			}
			else {
				matrR[i][j] = 0;
			}
		}
	}

}




int main() {
	setlocale(LC_ALL, "Russian");

	cout << setprecision(5);


	//Разложение QR
	
	vector<vector<double> >matrix(N, vector<double>(N, 0));


	cout << "Введите матрицу 3x3: \n";

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			cin >> matrix[i][j];
		}
	}


	double eps;

	cout << "Введите макс. погрешность (точность): \n";

	cin >> eps;

	vector<vector<double> >matrQ(N, vector<double>(N, 0));

	vector<vector<double> >matrR(N, vector<double>(N, 0));

	getQR(matrix, matrQ, matrR);

	bool trigger = true;

	int iters = 1;
	
	while (trigger) {
		vector<vector<double> >invA(N, vector<double>(N, 0));

		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				for (int k = 0; k < N; ++k) {
					invA[i][j] += matrR[i][k] * matrQ[k][j];
				}
			}
		}

		if (abs(invA[1][0]) < eps && abs(invA[2][0]) < eps) {
			trigger = false;
		}

		if (trigger) {
			getQR(invA, matrQ, matrR);
			++iters;
		}

	}

	vector<vector<double> >ansMatrix(N, vector<double>(N, 0));

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			for (int k = 0; k < N; ++k) {
				ansMatrix[i][j] += matrR[i][k] * matrQ[k][j];
			}
		}
	}


	vector<Imag> answers(0);

	answers.push_back(Imag(ansMatrix[0][0]));


	//Решение квадратного уравнения

	double A = 1;

	double B = -ansMatrix[1][1] - ansMatrix[2][2];

	double C = ansMatrix[1][1] * ansMatrix[2][2] - ansMatrix[1][2] * ansMatrix[2][1];

	Imag iA = Imag(A);
	Imag iB = Imag(B);
	Imag iC = Imag(C);

	Imag D = iB * iB - 4 * iA * iC;

	Imag Answer2 = (-iB + Imag::sqrt(D)) / (2 * iA);

	Imag Answer3 = (-iB - Imag::sqrt(D)) / (2 * iA);


	answers.push_back(Answer2);
	answers.push_back(Answer3);


	cout << "\nСобственные значения:\n";

	for (int i = 0; i < answers.size(); ++i) {
		cout << answers[i] << "\n";
	}


	cout << "\nКоличество итераций: " << iters << "\n";

}