#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace lab_bigint {
class bigint {
private:
    std::vector<int> digits;

public:
    bigint() {
    }

    // cppcheck-suppress[noExplicitConstructor]
    bigint(unsigned int number) {
        while (number != 0) {
            digits.push_back(number % BASE);
            number = number / BASE;
        }
    }

    // cppcheck-suppress[noExplicitConstructor]
    bigint(const std::string &s) {
        int i = 0;
        if (s != "0") {
            while (s[i] == '0') {
                i++;
            }
        }
        int digit = 0;
        int base = 1;
        for (int j = s.size() - 1; j >= i; j--) {
            digit += (s[j] - '0') * base;
            base *= 10;
            if (base == BASE || j == i) {
                digits.push_back(digit);
                digit = 0;
                base = 1;
            }
        }
    }

    friend bool operator==(const bigint &lhs, const bigint &rhs) {
        return lhs.digits == rhs.digits;
    }

    friend bool operator<(const bigint &lhs, const bigint &rhs) {
        std::vector<int> lhs_bigint = lhs.digits;
        std::vector<int> rhs_bigint = rhs.digits;
        if (lhs_bigint.size() < rhs_bigint.size()) {
            return true;
        }
        if (lhs_bigint.size() > rhs_bigint.size()) {
            return false;
        }
        for (int i = lhs_bigint.size() - 1; i >= 0; i--) {
            if (lhs_bigint[i] > rhs_bigint[i]) {
                return false;
            }
            if (lhs_bigint[i] < rhs_bigint[i]) {
                return true;
            }
        }
        return false;
    }

    std::string to_string() const {
        std::string answer = "";
        int zeros = std::to_string(BASE).length() - 1;
        int size = digits.size() - 1;
        for (int i = digits.size() - 1; i >= 0; i--) {
            if (i < size) {
                for (std::size_t j = 0;
                     j < zeros - std::to_string(digits[i]).length(); j++) {
                    answer += '0';
                }
            }
            answer += std::to_string(digits[i]);
        }
        return answer;
    }

    friend bigint &operator+=(bigint &lhs, bigint rhs) {
        for (std::size_t i = 0; i < rhs.digits.size(); i++) {
            if (i < lhs.digits.size()) {
                lhs.digits[i] += rhs.digits[i];
                if (lhs.digits[i] >= BASE) {
                    std::size_t j = i;
                    while (j < lhs.digits.size() - 1 && lhs.digits[j] >= BASE) {
                        lhs.digits[j] = lhs.digits[j] - BASE;
                        lhs.digits[j + 1] += 1;
                        j++;
                    }
                    if (lhs.digits[j] >= BASE) {
                        lhs.digits[j] = lhs.digits[j] - BASE;
                        lhs.digits.push_back(1);
                    }
                }
            } else {
                lhs.digits.push_back(rhs.digits[i]);
            }
        }

        return lhs;
    }

    friend bigint &operator-=(bigint &lhs, bigint rhs) {
        for (std::size_t i = 0;
             i < std::min(lhs.digits.size(), rhs.digits.size()); i++) {
            lhs.digits[i] = lhs.digits[i] - rhs.digits[i];
            if (lhs.digits[i] < 0) {
                std::size_t j = i;

                while (j < lhs.digits.size() - 1 && lhs.digits[j] < 0) {
                    lhs.digits[j] += BASE;
                    lhs.digits[j + 1] = lhs.digits[j + 1] - 1;
                    j++;
                }
            }
        }
        std::size_t i = lhs.digits.size() - 1;
        while (lhs.digits.size() > 1 && lhs.digits[i] == 0) {
            lhs.digits.pop_back();
            i--;
        }
        return lhs;
    }

    bigint &operator++() {
        return (*this += 1);
    }

    bigint operator++(int) {
        bigint old = *this;
        ++*this;
        return old;
    }

    bigint &operator--() {
        return (*this -= 1);
    }

    bigint operator--(int) {
        bigint old = *this;
        --*this;
        return old;
    }

    explicit operator unsigned int() const {
        unsigned int answer_int = 0;
        int j = 1;
        for (auto c : digits) {
            answer_int += c * j;
            j *= BASE;
        }
        return answer_int;
    }

    friend std::ostream &operator<<(std::ostream &os, const bigint &output) {
        std::string output_string = output.to_string();
        os << output_string;
        return os;
    }

    friend std::istream &operator>>(std::istream &is, bigint &input) {
        std::string input_string;
        is >> input_string;
        input.digits = bigint(input_string).digits;
        return is;
    }
};

bool operator!=(const bigint &lhs, const bigint &rhs) {
    return !(lhs == rhs);
}

bool operator>=(const bigint &lhs, const bigint &rhs) {
    return !(lhs < rhs);
}

bool operator>(const bigint &lhs, const bigint &rhs) {
    return !(lhs < rhs || lhs == rhs);
}

bool operator<=(const bigint &lhs, const bigint &rhs) {
    return !(lhs > rhs);
}

bigint operator+(const bigint &lhs, const bigint &rhs) {
    bigint lhs_copy = lhs;
    return lhs_copy += rhs;
}

bigint operator-(const bigint &lhs, const bigint &rhs) {
    bigint lhs_copy = lhs;
    return lhs_copy -= rhs;
}

}  // namespace lab_bigint
