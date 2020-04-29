#ifndef TWORKMATRIX_H
#define TWORKMATRIX_H

#include <iostream>
#include <vector>
#include "TDrob.h"

void DivideBy(std::vector<TDrob>& row, TDrob divider);
void MultBy(std::vector<TDrob>& row, TDrob mult);
void SetRow(std::vector<TDrob>& row1, const std::vector<TDrob> row2);
void PlusRow(std::vector<TDrob>& row1, const std::vector<TDrob> row2);
void MinusRow(std::vector<TDrob>& row1, const std::vector<TDrob> row2);
void ToDiag(std::vector<std::vector<TDrob> >& matrix);
void Decrement(std::vector<std::vector<TDrob> >& matrix);
void Normirovka(std::vector<std::vector<TDrob> >& matrix);
void ToE(std::vector<std::vector<TDrob> >& matrix);
void PrintMatrix(std::vector<std::vector<TDrob> >& matrix);
bool NoNullDiag(const std::vector<std::vector<TDrob> >& matrix);
int FindNullDiag(const std::vector<std::vector<TDrob> >& matrix);

#endif