#include "BigInt.h"

namespace NBigInt {
	TBigInt::TBigInt() : numbers{ 0 } {}


	const TBigInt& TBigInt::operator+=(const TBigInt& rhs) {
		return ((*this) = ((*this) + rhs));
	}

	TBigInt operator+(const TBigInt& lhs, const TBigInt& rhs) {
		TBigInt result = lhs;
		if (rhs.numbers.size() > result.numbers.size()) {
			result.numbers.resize(rhs.numbers.size());
		}

		bool ost = 0;

		for (size_t i = 0; i < result.numbers.size(); ++i) {
			result.numbers[i] += (rhs.getDigit(i) + ost);

			if (result.numbers[i] >= TBigInt::number_base) {
				ost = 1;
			}
			else {
				ost = 0;
			}

			if (ost) {
				result.numbers[i] -= TBigInt::number_base;
			}
			else if (i >= rhs.numbers.size())
				break;
		}

		if (ost) result.numbers.push_back(1);

		return result;
	}


	const TBigInt& TBigInt::operator-=(const TBigInt& rhs) {
		return ((*this) = ((*this) - rhs));
	}

	TBigInt operator-(const TBigInt& lhs, const TBigInt& rhs) {
		TBigInt result = lhs;
		if (rhs > lhs) {
			throw std::invalid_argument("rhs > lhs");
		}

		bool ost = 0;

		for (size_t i = 0; i < result.numbers.size(); ++i) {
			result.numbers[i] -= (rhs.getDigit(i) + ost);

			if (result.numbers[i] < 0) {
				ost = 1;
			}
			else {
				ost = 0;
			}

			if (ost) {
				result.numbers[i] += TBigInt::number_base;
			}
			else if (i >= rhs.numbers.size())
				break;
		}
		result.removeZeros();
		return result;
	}

	const TBigInt& TBigInt::operator*=(const TBigInt& rhs) {
		return ((*this) = ((*this) * rhs));
	}


	TBigInt operator*(const TBigInt& lhs, const TBigInt& rhs) {
		size_t lhs_size = lhs.numbers.size();
		size_t rhs_size = rhs.numbers.size();

		TBigInt result;
		result.numbers.resize(lhs_size + rhs_size);

		for (size_t i = 0; i < lhs_size; ++i) {
			int ost = 0;
			for (size_t j = 0; j < rhs_size || ost; ++j) {
				result.numbers[i + j] += lhs.numbers[i] * rhs.getDigit(j) + ost;
				ost = result.numbers[i + j] / TBigInt::number_base;
				result.numbers[i + j] -= (ost * TBigInt::number_base);
			}
		}

		result.removeZeros();

		return result;
	}

	const TBigInt& TBigInt::operator/=(const TBigInt& rhs) {
		return ((*this) = ((*this) / rhs));
	}


	TBigInt operator/(const TBigInt& lhs, const TBigInt& rhs) {
		long long d = TBigInt::number_base / (rhs.numbers.back() + 1);
		TBigInt u = lhs * TBigInt(d);
		TBigInt v = rhs * TBigInt(d); 
		size_t uSize = u.size();
		TBigInt q;
		q.resize(uSize);
		TBigInt r;
		for (int i = uSize - 1; i >= 0; --i) {
			r *= TBigInt::number_base;
			r += u.get(i);
			long long tmp1 = (r.size() <= v.size() ? 0 : r.get(v.size()));
			long long tmp2 = (r.size() <= v.size() - 1 ? 0 : r.get(v.size() - 1));
			long long q2 = (tmp1 * TBigInt::number_base + tmp2) / v.numbers.back();
			TBigInt tmp = v * TBigInt(q2);

			while (tmp > r) {
				tmp = tmp - v;
				--q2;
			}
			r = r - tmp;
			q.set(i, q2);
		}
		q.removeZeros();
		return q;
	}

	bool operator==(const TBigInt& lhs, const TBigInt& rhs) {
		return lhs.numbers == rhs.numbers;
	}

	bool operator<(const TBigInt& lhs, const TBigInt& rhs) {
		if (lhs.numbers.size() != rhs.numbers.size()) {
			return lhs.numbers.size() < rhs.numbers.size();
		}
		return std::lexicographical_compare(lhs.numbers.rbegin(), lhs.numbers.rend(),
			rhs.numbers.rbegin(), rhs.numbers.rend());
	}

	bool operator<=(const TBigInt& lhs, const TBigInt& rhs) {
		return (lhs < rhs) || (lhs == rhs);
	}

	bool operator>(const TBigInt& lhs, const TBigInt& rhs) {
		return !(lhs <= rhs);
	}

	std::istream& operator>>(std::istream& is, TBigInt& obj) {
		std::istream::sentry check(is);
		if (check) {
			using input_iterator = std::istreambuf_iterator<char>;

			obj.numbers.clear();

			input_iterator it(is), eos;
			bool lead_zero_flag = true;

			std::stack<char> decimal_digits;

			while (it != eos && (!isspace(*it))) {
				if ((*it) <= '9' && (*it) >= '0') {
					if (!(lead_zero_flag && *it == '0')) {
						decimal_digits.push(*it);
						lead_zero_flag = false;
					}
				}
				else {
					is.setstate(std::ios::failbit);
					break;
				}
				++it;
			}

			std::stack<char> digit_stack;

			while (!decimal_digits.empty()) {
				digit_stack.push(decimal_digits.top());

				if (digit_stack.size() == TBigInt::number_size) {
					long long digit = 0;
					while (!digit_stack.empty()) {
						digit *= 10;
						digit += (long long)(digit_stack.top() - '0');
						digit_stack.pop();
					}
					obj.numbers.push_back(digit);
				}

				decimal_digits.pop();
			}

			if (!digit_stack.empty()) {
				long long digit = 0;
				while (!digit_stack.empty()) {
					digit *= 10;
					digit += (long long)(digit_stack.top() - '0');
					digit_stack.pop();
				}
				obj.numbers.push_back(digit);
			}
			if (lead_zero_flag) obj.numbers = { 0 };
		}
		return is;
	}

	std::ostream& operator<<(std::ostream& os, const TBigInt& obj) {
		for (auto it = obj.numbers.rbegin(); it != obj.numbers.rend(); ++it) {
			if (it != obj.numbers.rbegin()) {
				os << std::setw(TBigInt::number_size) << std::setfill('0');
			}
			os << *it;
		}
		return os;
	}

	TBigInt Pow(TBigInt num, TBigInt power) {
		if ((num == TBigInt(0)) && (power == TBigInt(0))) {
			throw std::invalid_argument("0 ^ 0");
		}
		TBigInt result(1);
		while (power) {
			if (power.Odd()) {
				result *= num;
			}
			num *= num;
			power /= TBigInt(2);
		}
		return result;
	}

}