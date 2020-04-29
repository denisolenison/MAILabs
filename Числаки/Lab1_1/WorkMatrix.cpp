#include "WorkMatrix.h"


void DivideBy(std::vector<TDrob>& row, TDrob divider) {
	for (int i = 0; i < row.size(); ++i) {
		row[i] /= divider;
	}
}
void MultBy(std::vector<TDrob>& row, TDrob mult) {
	for (int i = 0; i < row.size(); ++i) {
		row[i] *= mult;
	}
}

void SetRow(std::vector<TDrob>& row1, const std::vector<TDrob> row2) {
	for (int i = 0; i < row1.size(); ++i) {
		row1[i] = row2[i];
	}
}

void PlusRow(std::vector<TDrob>& row1, const std::vector<TDrob> row2) {
	for (int i = 0; i < row1.size(); ++i) {
		row1[i] += row2[i];
	}
}

void MinusRow(std::vector<TDrob>& row1, const std::vector<TDrob> row2) {
	for (int i = 0; i < row1.size(); ++i) {
		row1[i] -= row2[i];
	}
}


void ToDiag(std::vector<std::vector<TDrob> >& matrix) {
	for (int i = 0; i < matrix.size(); ++i) {
		if (matrix[i][i] != 0) {
			DivideBy(matrix[i], matrix[i][i]);
		}
	}
}



void Decrement(std::vector<std::vector<TDrob> >& matrix) {
	std::vector<TDrob> tempRow(matrix.size(), 0);
	for (int i = 0; i < matrix.size(); ++i) {
		for (int j = 0; j < matrix.size(); ++j) {
			if (matrix[i][i] != 0 && i != j) {
				SetRow(tempRow, matrix[i]);
				MultBy(tempRow, matrix[j][i] / matrix[i][i]);
				MinusRow(matrix[j], tempRow);
			}
		}
	}
}

void Normirovka(std::vector<std::vector<TDrob> >& matrix) {
	for (int i = 0; i < matrix.size(); ++i) {
		if (matrix[i][i] != 0) {
			DivideBy(matrix[i], matrix[i][i]);
		}
	}
}

void ToE(std::vector<std::vector<TDrob> >& matrix) {
	ToDiag(matrix);
	for (int i = 0; i < 5; ++i) {
		Decrement(matrix);
		Decrement(matrix);
	}
	Normirovka(matrix);
}


void PrintMatrix(std::vector<std::vector<TDrob> >& matrix) {
	for (int i = 0; i < matrix.size(); ++i) {
		for (int j = 0; j < matrix[i].size(); ++j) {
			std::cout << matrix[i][j] << '\t';
		}
		std::cout << '\n';
	}
}

bool NoNullDiag(const std::vector<std::vector<TDrob> >& matrix) {
	bool isTrue = true;
	for (int i = 0; i < matrix.size(); ++i) {
		if (matrix[i][i] == 0) {
			isTrue = false;
			break;
		}
	}
	return isTrue;
}

int FindNullDiag(const std::vector<std::vector<TDrob> >& matrix) {
	int nullDiag = 0;
	for (int i = 0; i < matrix.size(); ++i) {
		if (matrix[i][i] == 0) {
			nullDiag = i;
			break;
		}
	}
	return nullDiag;
}