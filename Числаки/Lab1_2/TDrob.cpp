#include "TDrob.h"
#include<algorithm>


TDrob::TDrob(long long dividend, long long divider) : dividend(dividend), divider(divider) {
}
TDrob::TDrob(long long number) : dividend(number), divider(1) {
}
TDrob::TDrob() : dividend(0), divider(1) {
}



long long TDrob::GCD(long long a, long long b) {
	long long newA = abs(a);
	long long newB = abs(b);
	while (newA != 0 && newB != 0) {
		(newA > newB ? newA %= newB : newB %= newA);
	}
	return newA + newB;
}


long long TDrob::LCM(long long a, long long b) {
	return a / TDrob::GCD(a, b) * b;
}


void TDrob::Shrink(TDrob& drob) {
	long long gcd = TDrob::GCD(drob.dividend, drob.divider);
	drob.dividend /= gcd;
	drob.divider /= gcd;
}

void TDrob::Normalize(TDrob& drob) {
	if (drob.divider < 0) {
		drob.dividend = -drob.dividend;
		drob.divider = -drob.divider;
	}
	if (drob.divider == 0) drob.divider = 1;
}

bool TDrob::IsZero(const TDrob& drob) {
	return (drob.dividend == 0 ? 1 : 0);
}

double TDrob::ToDouble(const TDrob& drob) {
	return (double)(drob.dividend) / drob.divider;
}