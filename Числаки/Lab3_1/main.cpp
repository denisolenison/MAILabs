#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>

using namespace std;



int main() {
	setlocale(LC_ALL, "Russian");
	cout << "¬ариант 7, функци€ sqrt(x)\n";
	cout << "»нтерпол€ционный многочлен Ћагранджа:\n";
	int N = 4;
	cout << "¬ведите точки:\n";
	vector<double>points_L(N, 0);

	for (int i = 0; i < N; ++i) {
		cin >> points_L[i];
	}

	vector<double>values_L(N, 0);
	vector<double>Ws_L(N, 0);

	for (int i = 0; i < N; ++i) {
		values_L[i] = sqrt(points_L[i]);
	}

	for (int i = 0; i < N; ++i) {
		double base = 1;
		for (int j = 0; j < N; ++j) {
			if (i != j) {
				base *= (points_L[i] - points_L[j]);
			}
		}
		Ws_L[i] = base;
	}

	vector<double>FdivW(N, 0);
	for (int i = 0; i < N; ++i) {
		FdivW[i] = values_L[i] / Ws_L[i];
	}

	cout << "\n»нтерпол€ционный многочлен Ћагранджа:\n\n";

	bool firstWas = false;

	for (int i = 0; i < N; ++i) {
		if (firstWas) {
			if (FdivW[i] > 0) {
				cout << " + " << FdivW[i];
			}
			else if (FdivW[i] < 0) {
				cout << " - " << abs(FdivW[i]);
			}
			else {
				continue;
			}
		}
		else {
			if (FdivW[i] != 0) {
				cout << FdivW[i];
				firstWas = true;
			}
			else {
				continue;
			}
		}
		for (int j = 0; j < N; ++j) {
			if (i != j) {
				cout << "(x";
				if (points_L[j] > 0) {
					cout << " - " << points_L[j];
				}
				else if (points_L[j] < 0) {
					cout << " + " << -points_L[j];
				}
				cout << ")";
			}
		}
	}


	double X_star = 3.0;

	cout << "\n\nX* = " << X_star << "\n\n";

	double L_value = 0;

	for (int i = 0; i < N; ++i) {
		double basI = FdivW[i];
		for (int j = 0; j < N; ++j) {
			if (i != j) {
				basI *= (X_star - points_L[j]);
			}
		}
		L_value += basI;
	}

	cout << "L" << (N - 1) << "(X*) = " << L_value << "\n";
	cout << "Sqrt(" << X_star << ") = " << sqrt(X_star) << "\n\n";
	cout << "јбсолютна€ погрешность = " << abs(sqrt(X_star) - L_value) << "\n";


	cout << "\n»нтерпол€ционный многочлен Ќьютона:\n";
	cout << "¬ведите точки:\n";
	vector<double>points_N(N, 0);

	for (int i = 0; i < N; ++i) {
		cin >> points_N[i];
	}

	vector<double>values_N(N, 0);
	

	vector<vector<double>>getVals_N(N, vector<double>(0));

	for (int i = 0; i < N; ++i) {
		values_N[i] = sqrt(points_N[i]);
		getVals_N[0].push_back(values_N[i]);
	}

	int iter = 0;

	while (getVals_N[iter].size() > 1) {
		int M = getVals_N[iter].size();
		for (int i = 0; i < M - 1; ++i) {
			getVals_N[iter + 1].push_back((getVals_N[iter][i + 1] - getVals_N[iter][i]) / (points_N[iter + i + 1] - points_N[i]));
		}
		iter++;
	}
	cout << "\n»нтерпол€ционный многочлен Ќьютона:\n\n";

	firstWas = false;

	for (int i = 0; i < N; ++i) {
		if (firstWas) {
			if (getVals_N[i][0] > 0) {
				cout << " + " << getVals_N[i][0];
			}
			else if (getVals_N[i][0] < 0) {
				cout << " - " << abs(getVals_N[i][0]);
			}
			else {
				continue;
			}
		}
		else {
			if (getVals_N[i][0] != 0) {
				cout << getVals_N[i][0];
				firstWas = true;
			}
			else {
				continue;
			}
		}
		for (int j = 0; j < i; ++j) {
			cout << "(x";
			if (points_N[j] > 0) {
				cout << " - " << points_N[j];
			}
			else if (points_N[j] < 0) {
				cout << " + " << -points_N[j];
			}
			cout << ")";
		}
	}


	cout << "\n\nX* = " << X_star << "\n\n";

	double N_value = 0;


	for (int i = 0; i < N; ++i) {
		double basI = getVals_N[i][0];
		for (int j = 0; j < i; ++j) {
			basI *= (X_star - points_N[j]);
		}
		N_value += basI;
	}

	cout << "P" << (N - 1) << "(X*) = " << N_value << "\n";
	cout << "Sqrt(" << X_star << ") = " << sqrt(X_star) << "\n\n";
	cout << "јбсолютна€ погрешность = " << abs(sqrt(X_star) - N_value) << "\n";

}