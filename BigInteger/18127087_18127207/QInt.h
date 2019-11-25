#pragma once
#include <iostream>
#include <bitset>
#include <string>

using namespace std;

class QInt
{
private:
	bitset<128> arrayBits;

public:
	// Constructors
	QInt();
	QInt(const QInt&);

	// Base Conversion Methods
	void toBase2(string base);
	string toBase10();
	string toBase16();

	// Unary Operators
	// Support Operators
	QInt& operator=(const QInt&);

	// Bitwise
	QInt operator~();

	// Binary Operators
	// Arithmetic
	QInt operator+(const QInt&);
	QInt operator-(const QInt&);
	//QInt operator*(const QInt&);
	//QInt operator/(const QInt&);

	//// Bitwise
	QInt operator<<(const uint8_t&);
	QInt operator>>(const uint8_t&);
	//QInt rol();
	//QInt ror();

	QInt operator& (const QInt&);
	QInt operator| (const QInt&);
	QInt operator^ (const QInt&);
};