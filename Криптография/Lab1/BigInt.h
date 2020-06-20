#ifndef BIGINT_H
#define BIGINT_H

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <stack>
#include <vector>
#include <string>

namespace NBigInt {

	class TBigInt {

		static const long long number_base = 10'000'000;
		static const size_t number_size = 7;


		std::vector<long long> numbers;

		inline size_t size() const { return numbers.size(); }

		long long getDigit(size_t i) const {
			if (i < numbers.size()) {
				return numbers[i];
			}
			else {
				return 0;
			}
		}

		void removeZeros() {
			size_t remover = numbers.size();
			for (size_t i = numbers.size() - 1; i != 0; --i) {
				if (numbers[i] != 0) break;
				remover = i;
			}
			numbers.resize(remover);
			if (numbers.empty()) numbers.push_back(0);
		}

	public:
		TBigInt();
		TBigInt(const TBigInt&) = default;

		TBigInt(long long num) {
			while (num > 0) {
				numbers.push_back(num % number_base);
				num /= number_base;
			}
			if (numbers.empty()) numbers.push_back(0);
		}


		TBigInt(const std::string& str) {
			for (int i = str.size() - 1; i >= 0; --i) {
				int tmp = 0;
				for (int j = 0; i >= 0 && j < 7; ++j, --i) {
					tmp = tmp + (str[i] - '0') * pow(10, j);
				}
				++i;
				numbers.push_back(tmp);
			}
			removeZeros();
		}

		bool Odd() const {
			return TBigInt::numbers[0] % 2;
		}

		TBigInt& operator=(const TBigInt&) = default;

		const TBigInt& operator+=(const TBigInt&);
		friend TBigInt operator+(const TBigInt&, const TBigInt&);

		const TBigInt& operator-=(const TBigInt&);
		friend TBigInt operator-(const TBigInt&, const TBigInt&);

		const TBigInt& operator*=(const TBigInt&);
		friend TBigInt operator*(const TBigInt&, const TBigInt&);

		const TBigInt& operator/=(const TBigInt&);
		friend TBigInt operator/(const TBigInt&, const TBigInt&);

		friend bool operator==(const TBigInt&, const TBigInt&);
		friend bool operator<(const TBigInt&, const TBigInt&);
		friend bool operator<=(const TBigInt&, const TBigInt&);
		friend bool operator>(const TBigInt&, const TBigInt&);

		int get(size_t i) const { return numbers[i]; };
		void set(size_t i, long long num) { 
			numbers[i] = num;
		};

		void resize(const size_t& size) {
			numbers.resize(size, 0);
		}


		operator bool() const {
			if ((TBigInt::numbers.size() > 1) || (TBigInt::numbers[0] > 0)) return true;
			return false;
		}

		friend std::istream& operator>>(std::istream&, TBigInt&);
		friend std::ostream& operator<<(std::ostream&, const TBigInt&);

		~TBigInt() = default;
	};

	TBigInt Pow(TBigInt, TBigInt);

}

#endif