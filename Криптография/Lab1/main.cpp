#include "BigInt.h"
#include <iomanip>
using namespace std;

NBigInt::TBigInt GCD(NBigInt::TBigInt a, NBigInt::TBigInt b) {
	while (a != 0) {
		if (a < b) {
			NBigInt::TBigInt tmp = a;
			a = b;
			b = tmp;
		}
		NBigInt::TBigInt k = a / b;
		a = a - b * k;
	}
	return b;
}

int stoi(const string& str) {
	int tmp = 0;
	for (int i = 0; i < str.size(); ++i) {
		tmp = tmp * 10 + (str[i] - '0');
	}
	return tmp;
}

int main() {

	freopen("input.txt", "r", stdin);
	string stringNum1;
	cin >> stringNum1;
	NBigInt::TBigInt ishodnick(stringNum1);


	for (int i = 0; i < 19; ++i) {
		string stringNum2;
		cin >> stringNum2;
		NBigInt::TBigInt number2(stringNum2);
		NBigInt::TBigInt res = GCD(ishodnick, number2);
		if (i >= 7) {
			cout << "\n\n Variant " << i + 1;
		}
		else {
			cout << "\n\n Variant " << i;
		}
		cout << " GCD = " << res << "\n\n";
	}


	//Number one нашли сверху и скопировали
	NBigInt::TBigInt numberOne("130473868032583609827185448980364758181025721979158584058510968480674675318007582512991438379409900025634206602999335047253958597645751922572319519736549021098387734031741968386885087469564811369549756562791721156060671611860553422489104596835427639417952364878918433203601720734649355543293580048106131166649");
	NBigInt::TBigInt numberTwo = ishodnick / numberOne;
	cout << "\n\nFirst Divider = " << numberOne << "\n";
	cout << "\n\nSecond Divider = " << numberTwo << "\n\n";
	//проверим равенство чисел
	if ((numberOne * numberTwo) == ishodnick) {
		cout << "Vse Okey ;)\n";
	}
	else {
		cout << "Smthng wrong\n";
	}
	cout << "\n\n";

	while (true) { //чтобы окно не закрывалось
		int wait;
		cin >> wait;
	}
	return 0;

}