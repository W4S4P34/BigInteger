#include "QInt.h"
QInt QInt::operator&(const QInt& a) {
	QInt b;
	QInt c;
	for (int i = 0; i < a.arrayBits.size() - 1; i++) {
		c.arrayBits[i] = b.arrayBits[i] & a.arrayBits[i];
	}
	return c;
}
QInt QInt::operator|(const QInt& a) { 
	QInt b;
	QInt c;
	for (int i = 0; i < a.arrayBits.size() - 1; i++) {
		c.arrayBits[i] = b.arrayBits[i] | a.arrayBits[i];
	}
	return c;
}
QInt QInt::operator^(const QInt& a) {
	QInt b;
	QInt c;
	for (int i = 0; i < a.arrayBits.size() - 1; i++) {
		c.arrayBits[i] = b.arrayBits[i] ^ a.arrayBits[i];
	}
	return c;
}
QInt QInt::operator~() {
	QInt b;
	for (int i = 0; i < b.arrayBits.size() - 1; i++) {
		~b.arrayBits[i];
	}
	return b;
}