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
	void toBase2_10(string base);
	void toBase2_16(string base);
	string toBase10();
	string toBase16();

	// Support Conversion Methods
	// Set/Get Property
	string getBits();

	// From 10 To 2 Methods
	bool oddsToOne(string base);
	string divideByTwo(string base);

	// From 16 To 2 Methods
	string convertOneHexToBin(char base);

	// From 2 to 10 Methods
	string powerOfTwo(int power);

	// From 2 To 16 Methods
	string convertBinChunkToHex(string chunk);

	// Unary Operators
	// Support Operators
	QInt& operator=(const QInt&);

	// Bitwise
	QInt operator~();

	// Binary Operators
	// Arithmetic
	QInt operator+(const QInt&);
	QInt operator-(const QInt&);
	QInt operator*(const QInt&);
	QInt operator/(const QInt&);

	// Bitwise
	QInt operator<<(const int8_t&);
	QInt operator>>(const int8_t&);
	QInt rol();
	QInt ror();

	QInt operator& (const QInt&);
	QInt operator| (const QInt&);
	QInt operator^ (const QInt&);
};