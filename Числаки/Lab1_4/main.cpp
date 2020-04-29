#include<iostream>
#include<vector>
#include<cmath>



using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");

	int i, j;

	int n = 3;

	vector<vector<double> >A(n, vector<double>(n, 0));

	cout << "Введите матрицу 3х3: \n";

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			cin >> A[i][j];
		}
	}




	double f;
	double eps = 0.3;

	cout << "Введите макс. погрешность: ";
	cin >> eps;

	cout << "\n\n";
	

	double t = 0;

	int iter = 0;

	vector<vector<double> >V(n, vector<double>(n, 0));

	for (int i = 0; i < n; ++i) {
		V[i][i] = 1;
	}


	do {
		int maxi = 0, maxj = 0;

		double max = 0;

		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				if (i < j && abs(A[i][j]) > max) {
					max = abs(A[i][j]);
					maxi = i;
					maxj = j;
				}
			}
		}

		f = 0.5 * atan(2 * A[maxi][maxj] / (A[maxi][maxi] - A[maxj][maxj]));

		vector<vector<double> >U(n, vector<double>(n, 0));

		U[maxi][maxj] = -sin(f);
		U[maxj][maxi] = sin(f);
		U[maxi][maxi] = cos(f);
		U[maxj][maxj] = cos(f);

		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				if (i != j && i != maxi && j != maxj && i != maxj && j != maxi) {
					U[i][j] = 0;
				}
			}
		}

		for (int m = 0; m < n; m++) {
			if (m != maxi && m != maxj) {
				U[m][m] = 1;
			}
		}

		vector<vector<double> > matrixtrans(n, vector<double>(n, 0));
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				matrixtrans[j][i] = U[i][j];
			}
		}


		vector<vector<double> > C(n, vector<double>(n, 0));

		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				for (int k = 0; k < n; k++) {
					C[i][j] += matrixtrans[i][k] * A[k][j];
				}
			}
		}

		vector<vector<double> >A1(n, vector<double>(n, 0));

		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				for (int k = 0; k < n; k++) {
					A1[i][j] += C[i][k] * U[k][j];
				}
				A[i][j] = A1[i][j];
			}
		}

		double b = 0;

		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				if (i != j) {
					b += 0.5 * pow(A1[i][j], 2);
				}
			}
		}

		t = pow(b, 0.5);

		iter++;


		vector<vector<double> >newV(n, vector<double>(n, 0));

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				for (int k = 0; k < n; ++k) {
					newV[i][j] += V[i][k] * U[k][j];
				}
			}
		}

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				V[i][j] = newV[i][j];
			}
		}


	} while (t > eps);


	for (int i = 0; i < n; ++i) {
		double minVal = 999999999999;
		for (int j = 0; j < n; ++j) {
			if (abs(V[j][i]) < abs(minVal)) {
				minVal = V[j][i];
			}
		}
		for (int j = 0; j < n; ++j) {
			V[j][i] /= minVal;
		}
	}


	vector<double> lyamda(n);

	for (int i = 0; i < n; ++i) {
		lyamda[i] = A[i][i];
	}

	cout << "Собственные значения: ";
	for (int i = 0; i < n; ++i) {
		cout << lyamda[i];
		if (i != n - 1) {
			cout << ", ";
		}
	}

	cout << "\n\nСобственные вектора: \n";
	for (int i = 0; i < n; i++) {
		cout << "\nC" << i << "^T = {";
		for (int j = 0; j < n; ++j) {
			cout << V[j][i];
			if (j != n - 1) {
				cout << ", ";
			}
		}
		cout << "}";
	}
	cout << "\n\n";
}