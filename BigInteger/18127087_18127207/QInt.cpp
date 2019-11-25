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

QInt QInt::operator<<(const int8_t&) {
	for (int i = this->arrayBits.size() - 1; i >= 0; i--) {
		this->arrayBits[i] << 1;
	}
	return *this;
}

QInt QInt::operator>>(const int8_t&) {
	for (int i = this->arrayBits.size() - 2; i >= 0; i--) {
		this->arrayBits[i] >> 1;
	}
	return *this;
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
