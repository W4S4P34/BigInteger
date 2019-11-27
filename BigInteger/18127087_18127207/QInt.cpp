#include "QInt.h"

QInt::QInt()
{

}

QInt::QInt(const QInt& qi)
{
	for (int i = 0; i < 128; i++)
	{
		this->arrayBits[i] = qi.arrayBits[i];
	}
}
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
	temp.arrayBits.set(127, 1);
	OneComplement.arrayBits.flip();
	TwoComplement = temp + OneComplement;
	tempQi = *this + TwoComplement;
	return tempQi;
}
QInt QInt::operator*(const QInt& Qi) {
	QInt tempQi;
	QInt tempThis = *this;
	int countQi = 0, countThis = 0;
	for (int i = 0; i < (int)Qi.arrayBits.size(); i++) {
		if (Qi.arrayBits[i] == 1)
			countQi = ((int)Qi.arrayBits.size() - 1) - i;
		if (countQi != 0)
			break;
	}
	for (int i = 0; i < (int)tempThis.arrayBits.size(); i++) {
		if (this->arrayBits[i] == 1)
			countThis = ((int)tempThis.arrayBits.size() - 1) - i;
		if (countThis != 0)
			break;
	}
	QInt temp;
	int count = (countQi >= countThis) ? countQi : countThis;
	for (int i = (int)Qi.arrayBits.size() - 1; i >= ((int)Qi.arrayBits.size() - 1) - count; i--) {
		if (Qi.arrayBits[i] == 1) {
			temp = tempThis << ((int)Qi.arrayBits.size() - 1) - i;
		}
		else continue;
		tempQi = tempQi + temp;
	}
	return tempQi;
}
QInt QInt::operator/(const QInt& Qi) {
	bool NegaFlag = false;
	QInt tempDivident = *this, tempDivisor = Qi;
	if (this->arrayBits[0] == Qi.arrayBits[0]) {
		if (this->arrayBits[0] == 1) {
			tempDivident = tempDivident.toTwoComplement();
			tempDivisor = tempDivisor.toTwoComplement();
		}
	}
	else {
		NegaFlag = true;
		if (this->arrayBits[0] == 1) {
			tempDivident = tempDivident.toTwoComplement();
		}
		else if (Qi.arrayBits[0] == 1) {
			tempDivisor = tempDivisor.toTwoComplement();
		}
	}
	int NumberOfDividentBits = 128;
	int Holder;
	QInt Remainder;
	while (NumberOfDividentBits > 0) {
		Holder = tempDivident.arrayBits[0];
		tempDivident = tempDivident << 1;
		Remainder = Remainder << 1;
		Remainder.arrayBits.set(127, Holder);
		QInt tempPrevRemainder = Remainder;
		Remainder = Remainder - tempDivisor;
		if (Remainder.arrayBits[0] == 0) {
			tempDivident.arrayBits.set(127, 1);
		}
		else {
			tempDivident.arrayBits.set(127, 0);
			Remainder = tempPrevRemainder;
		}
		NumberOfDividentBits--;
	}
	if (NegaFlag)
		tempDivident = tempDivident.toTwoComplement();
	return tempDivident;
}
/// Bitwise

QInt QInt::operator<<(const uint8_t& n) {
	QInt temp = *this;
	for (uint8_t i = 0; i < (uint8_t)temp.arrayBits.size(); i++) {
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
QInt QInt::ror() {
	QInt tempQi;
	int carryBit = this->arrayBits[127];
	tempQi = *this >> 1;
	tempQi.arrayBits.set(0, carryBit);
	return tempQi;
}
QInt QInt::rol() {
	QInt tempQi;
	int carryBit = this->arrayBits[0];
	tempQi = *this << 1;
	tempQi.arrayBits.set(127, carryBit);
	return tempQi;
}
QInt QInt::operator^(const QInt& Qi) {
	QInt tempQi;
	for (int i = 0; i < Qi.arrayBits.size(); i++) {
		tempQi.arrayBits[i] = this->arrayBits[i] ^ Qi.arrayBits[i];
	}
	return tempQi;
}
QInt QInt::toTwoComplement() {
	QInt tempQi;
	QInt temp;
	temp.arrayBits.set(127, 1);
	tempQi = (~(*this)) + temp;
	return tempQi;
}
