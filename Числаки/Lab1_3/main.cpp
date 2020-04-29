#include<iostream>
#include<vector>
#include<cmath>
#include<cstdlib>

using namespace std;




void LUMethod(vector<vector<double> >matrix, vector<double>solve, int N, vector<double>& answer) {

	vector<vector<double> >matrixL(N, vector<double>(N, 0));
	vector<double>answerY(N, 0);

	for (int i = 0; i < N; ++i) {
		matrixL[i][i] = 1;
	}

	for (int r = 0; r < N - 1; ++r) {
		for (int i = r + 1; i < N; ++i) {
			double koef = matrix[i][r] / matrix[r][r];
			matrixL[i][r] = koef;
			for (int j = 0; j < N; ++j) {
				matrix[i][j] -= matrix[r][j] * koef;
			}
		}
	}

	answerY[0] = solve[0];
	for (int i = 1; i < N; ++i) {
		double value = 0;
		for (int j = 0; j < i; ++j) {
			value += answerY[j] * matrixL[i][j];
		}
		answerY[i] = solve[i] - value;
	}


	answer[N - 1] = answerY[N - 1] / matrix[N - 1][N - 1];
	for (int i = N - 2; i >= 0; --i) {
		double value = 0;
		for (int j = N - 1; j > i; --j) {
			value += answer[j] * matrix[i][j];
		}
		answer[i] = (answerY[i] - value) / matrix[i][i];
	}

}



void Iterations(vector<vector<double> >matrix, vector<double>solve, int N, vector<double> answer1, vector<double>& answer2, double& maxPogr, int& iterations) {
	bool pogrGot = false;
	bool pogrNew = false;

	vector<double>answerX(N, 0);

	while (!pogrGot) {
		for (int i = 0; i < N; ++i) {
			answerX[i] = solve[i] / matrix[i][i];
			for (int j = 0; j < N; ++j) {
				if (i != j) {
					answerX[i] -= matrix[i][j] / matrix[i][i] * answer2[j];
				}
			}
		}

		for (int i = 0; i < N; ++i) {
			answer2[i] = answerX[i];
		}

		++iterations;

		//Check pogr

		pogrNew = true;
		for (int i = 0; i < N; ++i) {
			if (abs(answer2[i] - answer1[i]) > maxPogr) {
				pogrNew = false;
			}
		}

		pogrGot = pogrNew;

	}


}



void Zeidell(vector<vector<double> >matrix, vector<double>solve, int N, vector<double> answer1, vector<double>& answer2, double& maxPogr, int& iterations) {
	bool pogrGot = false;
	bool pogrNew = false;

	while (!pogrGot) {
		for (int i = 0; i < N; ++i) {
			answer2[i] = solve[i] / matrix[i][i];
			for (int j = 0; j < N; ++j) {
				if (i != j) {
					answer2[i] -= matrix[i][j] / matrix[i][i] * answer2[j];
				}
			}
		}

		++iterations;

		//Check pogr

		pogrNew = true;
		for (int i = 0; i < N; ++i) {
			if (abs(answer2[i] - answer1[i]) > maxPogr) {
				pogrNew = false;
			}
		}

		pogrGot = pogrNew;

	}


}




int main() {
	setlocale(LC_ALL, "Russian");
	cout << "Введите размер матрицы\n";
	int N;
	cin >> N;
	vector<vector<double> >matrix(N, vector<double>(N, 0));

	vector<double>solve(N, 0);

	vector<double>answer1(N, 0);

	vector<double>answer2(N, 0);

	double maxPogr;
	int iterations = 0;
	int iterations2 = 0;


	vector<double>answer3(N, 0);


	cout << "Введите матрицу A\n";
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			cin >> matrix[i][j];
		}
	}
	cout << "Введите вектор B\n";
	for (int i = 0; i < N; ++i) {
		cin >> solve[i];
	}

	cout << "Введите максимальную погрешность\n";
	cin >> maxPogr;


	LUMethod(matrix, solve, N, answer1);

	Iterations(matrix, solve, N, answer1, answer2, maxPogr, iterations);

	Zeidell(matrix, solve, N, answer1, answer3, maxPogr, iterations2);


	cout << "\nОтвет (Метод LU):\n";
	for (int i = 0; i < N; ++i) {
		cout << answer1[i] << "\n";
	}

	cout << "\nОтвет (Метод простых итераций):\n";
	for (int i = 0; i < N; ++i) {
		cout << answer2[i] << "\n";
	}
	cout << "Количество итераций: " << iterations << "\n";

	cout << "\nОтвет (Метод Зейделя):\n";
	for (int i = 0; i < N; ++i) {
		cout << answer3[i] << "\n";
	}
	cout << "Количество итераций: " << iterations2 << "\n";



}