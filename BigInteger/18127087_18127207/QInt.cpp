#include "QInt.h"
//Support Operators
QInt& QInt::operator=(const QInt& Qi) {
	QInt tempQi;
	tempQi.arrayBits = Qi.arrayBits;
	return tempQi;
}

//Bitwise
QInt QInt::operator~() {
	QInt tempQi;
	tempQi.arrayBits.flip();
	return tempQi;
}

// Binary Operators
// Arithmetic
QInt QInt::operator+(const QInt& Qi) {
	bitset<1> temp = { 0 };
	QInt tempQi;
	for (int i = Qi.arrayBits.size() - 1; i >= 0; i--) {
		tempQi.arrayBits[i] = (this->arrayBits[i] ^ Qi.arrayBits[i]);
		if (this->arrayBits[i] == Qi.arrayBits[i]) {
			tempQi.arrayBits[i] = tempQi.arrayBits[i] ^ temp[0];
			if (Qi.arrayBits[i] == 0) {
				if (temp[0] == 1) {
					temp.flip();
				}
			}
			else {
				if (temp[0] == 0)
					temp.flip();
			}
		}
		else {
			if (this->arrayBits[i] == 1)
				tempQi.arrayBits[i] = this->arrayBits[i] ^ temp[0];
			else
				tempQi.arrayBits[i] = Qi.arrayBits[i] ^ temp[0];
		}
	}
	return tempQi;
}

QInt QInt::operator-(const QInt& Qi) {
	QInt tempQi, OneComplement, temp, TwoComplement;
	OneComplement = Qi;
	temp.arrayBits.set(0, 1);
	OneComplement.arrayBits.flip();
	TwoComplement = temp + OneComplement;
	tempQi = *this + TwoComplement;
	return tempQi;
}
/// Bitwise

QInt QInt::operator<<(const uint8_t& n) {
	QInt temp = *this;
	for (int i = 0; i < (int)temp.arrayBits.size(); i++) {
		if ((i + n) > temp.arrayBits.size() - 1) {
			temp.arrayBits.set(i, 0);
			continue;
		}
		temp.arrayBits[i] = temp.arrayBits[i + n];
	}
	return temp;
}

QInt QInt::operator>>(const uint8_t& n) {
	QInt tempQi = *this;
	int temp = tempQi.arrayBits[0];
	int count = 1;
	int bound = n;
	if (bound > 127)
		bound = 127;
	while (count <= bound) {
		for (int i = tempQi.arrayBits.size() - 1; i >= 1; i--) {
			tempQi.arrayBits[i] = tempQi.arrayBits[i - 1];
		}
		count++;
	}
	for (int i = count - 1; i >= 0; i--) {
		tempQi.arrayBits[i] = temp;
	}
	return tempQi;
}
QInt QInt::operator&(const QInt& Qi) {
	QInt tempQi;
	for (int i = 0; i < Qi.arrayBits.size(); i++) {
		tempQi.arrayBits[i] = this->arrayBits[i] & Qi.arrayBits[i];
	}
	return tempQi;
}

QInt QInt::operator|(const QInt& Qi) {
	QInt tempQi;
	for (int i = 0; i < Qi.arrayBits.size(); i++) {
		tempQi.arrayBits[i] = this->arrayBits[i] | Qi.arrayBits[i];
	}
	return tempQi;
}

QInt QInt::operator^(const QInt& Qi) {
	QInt tempQi;
	for (int i = 0; i < Qi.arrayBits.size(); i++) {
		tempQi.arrayBits[i] = this->arrayBits[i] ^ Qi.arrayBits[i];
	}
	return tempQi;
}
