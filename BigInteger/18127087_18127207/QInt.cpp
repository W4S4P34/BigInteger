#include "QInt.h"

// Constructors
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

/*************************************************************************/

void QInt::toBase2_2(string base)
{
	bool isActive = false;

	for (int i = 0; i < (int)base.length(); i++)
	{
		if (base[i] == '0') isActive = false;
		else isActive = true;

		this->arrayBits.set((long long int)127 - ((int)base.length() - 1) + i, isActive);
	}
}

// Base Conversion Methods
void QInt::toBase2_10(string base)
{
	if (base == "0") return;
	else
	{
		bool negativeFlag = false;
		// Erase the minus "-"
		if (base[0] == '-')
		{
			negativeFlag = true;
			base.erase(0, 1);
		}

		int i = 127; bool mode = 0;
		while (base != "0")
		{
			if (i == 0) break;

			mode = oddsToOne(base);
			this->arrayBits.set(i--, mode);
			base.assign(divideByTwo(base));
		}

		if (negativeFlag)
		{
			bool carryBit = false;
			this->arrayBits.flip();

			QInt bitOne; bitOne.toBase2_2("1");

			string tempString = this->arrayBits.to_string();
			string reversedString;

			for (int i = (int)tempString.length() - 1; i >= 0; i--)
			{
				reversedString += tempString[i];
			}

			QInt current; current.toBase2_2(reversedString);

			QInt twoComplement = current + bitOne;

			this->toBase2_2(twoComplement.getBits());
		}
	}
}

void QInt::toBase2_16(string base)
{
	if (base == "0") return;
	else
	{
		bool mode = 0; string tempString;
		for (int i = 124; i >= (int)(128 - (base.length() * 4)); i -= 4)
		{
			tempString = convertOneHexToBin(base[(long long int)((int)base.length() - 1) - ((long long int)31 - (i / 4))]);
			for (int j = i; j <= i + 3; j++)
			{
				mode = (tempString[(long long int)j - i] == '1');
				this->arrayBits.set(j, mode);
			}
		}
	}
}

string QInt::toBase10()
{
	string tempString;
	string newString(39, '0');
	// Create Summary Table
	char** conversionTable = new char* [128];
	for (int i = 0; i < 128; i++)
	{
		if (this->arrayBits[(long long int)127 - i])
		{
			tempString.assign(powerOfTwo(i));
		}
		else tempString.assign(39, '0');

		conversionTable[i] = new char[39]();
		for (int j = 0; j < 39; j++)
		{
			conversionTable[i][j] = tempString[j];
		}
	}

	// Sum the result
	int carryElement = 0, carryCapacity = 0;
	int tempResult = 0;
	int result[39] = { 0 };

	for (int i = 38; i >= 0; i--)
	{
		tempResult = 0;
		for (int j = 0; j < 127; j++)
		{
			tempResult += conversionTable[j][i] - 48;
		}
		carryCapacity += tempResult; carryElement = carryCapacity % 10; carryCapacity /= 10;
		result[i] = carryElement;
	}

	int temp = 0;
	carryElement = 0; carryCapacity = 0;
	if (this->arrayBits[0])
	{
		for (int i = 38; i >= 0; i--)
		{
			if ((conversionTable[127][i] - 48) < result[i])
			{
				temp = (conversionTable[127][i] - 48) + 10;
				carryCapacity = temp / 10;
				if (i > 0)
				{
					result[i - 1] += carryCapacity;
				}
			}
			else
			{
				temp = (conversionTable[127][i] - 48);
				carryCapacity = 0;
			}

			tempResult = temp - result[i];

			result[i] = tempResult;
		}
	}

	// Copy to string
	for (int i = 0; i < 39; i++)
	{
		newString.replace(i, 1, to_string(result[i]));
	}

	// Delete zeros
	size_t found = newString.find_first_not_of("0");

	if (found != string::npos)
	{
		newString.erase(0, found);

		// Check whether number positive or negative
		if (this->arrayBits[0])
		{
			newString.assign("-" + newString);
		}

		// Delete Summary Table
		for (int i = 0; i < 128; i++)
		{
			delete[] conversionTable[i];
		}
		delete[] conversionTable;

		return newString;
	}
	else
	{
		// Delete Summary Table
		for (int i = 0; i < 128; i++)
		{
			delete[] conversionTable[i];
		}
		delete[] conversionTable;

		return "0";
	}
}

string QInt::toBase16()
{
	string newString; string tempString;

	for (int i = 124; i >= 0; i -= 4)
	{
		tempString.clear();
		for (int j = i; j <= i + 3; j++)
		{
			tempString.append(to_string(this->arrayBits[j]));
		}
		newString.assign(convertBinChunkToHex(tempString) + newString);
	}

	size_t found = newString.find_first_not_of("0");

	if (found != string::npos)
	{
		newString.erase(0, found);
		return newString;
	}
	else
	{
		return "0";
	}
}

// Support Conversion Methods
// Converter
QInt QInt::toTwoComplement()
{
	QInt tempQi;
	QInt temp;
	temp.arrayBits.set(127, 1);
	tempQi = (~(*this)) + temp;
	return tempQi;
}

// Set/Get Property
string QInt::getBits()
{
	string tempString;

	for (int i = 0; i < (int)arrayBits.size(); i++)
	{
		tempString.append(to_string(arrayBits[i]));
	}

	size_t found = tempString.find_first_not_of("0");

	if (found != string::npos)
	{
		tempString.erase(0, found);
		return tempString;
	}
	else
	{
		return "0";
	}
}

// From 10 to 2 Methods
bool QInt::oddsToOne(string base)
{
	if (base[base.length() - 1] == '1'
		|| base[base.length() - 1] == '3'
		|| base[base.length() - 1] == '5'
		|| base[base.length() - 1] == '7'
		|| base[base.length() - 1] == '9')
	{
		return true;
	}
	else return false;
}

string QInt::divideByTwo(string base)
{
	string newString;
	int newDigit = 0;
	int addMod = 0;

	for (int i = 0; i < (int)base.length(); i++)
	{
		newDigit = (int)((base[i] - '0') / 2) + addMod;
		newString.assign(newString + to_string(newDigit));
		addMod = (int)oddsToOne((to_string(base[i]))) * 5;
	}

	if (newString != "0" && newString[0] == '0')
	{
		newString.assign(newString.begin() + 1, newString.end());
	}

	return newString;
}

// From 16 To 2 Methods
string QInt::convertOneHexToBin(char base)
{
	switch (base)
	{
	case '1': { return "0001"; break; }
	case '2': { return "0010"; break; }
	case '3': { return "0011"; break; }
	case '4': { return "0100"; break; }
	case '5': { return "0101"; break; }
	case '6': { return "0110"; break; }
	case '7': { return "0111"; break; }
	case '8': { return "1000"; break; }
	case '9': { return "1001"; break; }
	case 'A': { return "1010"; break; }
	case 'B': { return "1011"; break; }
	case 'C': { return "1100"; break; }
	case 'D': { return "1101"; break; }
	case 'E': { return "1110"; break; }
	case 'F': { return "1111"; break; }
	default: { return "0000"; break; }
	}
}

// From 2 to 10 Methods
string QInt::powerOfTwo(int power)
{
	string tempString(39, '0');
	tempString.replace(38, 1, "1");

	if (power == 0)
	{
		return tempString;
	}
	else
	{
		int stringIndex = 38;
		int carryElement = 0;
		int temp = 0;

		while (power > 0)
		{
			stringIndex = 38;
			while (stringIndex >= 0)
			{
				temp = 2 * (tempString[stringIndex] - 48) + carryElement;

				if (tempString[stringIndex] - 48 >= 5)
				{
					carryElement = 1;
				}
				else
				{
					carryElement = 0;
				}

				tempString.replace(stringIndex, 1, to_string(temp % 10));

				stringIndex--;
			}
			power--;
		}

		return tempString;
	}
}

// From 2 To 16 Methods
string QInt::convertBinChunkToHex(string chunk)
{
	if (chunk == "0001") return "1";
	else if (chunk == "0010") return "2";
	else if (chunk == "0011") return "3";
	else if (chunk == "0100") return "4";
	else if (chunk == "0101") return "5";
	else if (chunk == "0110") return "6";
	else if (chunk == "0111") return "7";
	else if (chunk == "1000") return "8";
	else if (chunk == "1001") return "9";
	else if (chunk == "1010") return "A";
	else if (chunk == "1011") return "B";
	else if (chunk == "1100") return "C";
	else if (chunk == "1101") return "D";
	else if (chunk == "1110") return "E";
	else if (chunk == "1111") return "F";
	else return "0";
}

/*************************************************************************/

// Unary Operators
// Support Operators
QInt& QInt::operator=(const QInt& Qi)
{
	if (this == &Qi) return *this;

	for (int i = 0; i < (int)Qi.arrayBits.size(); i++)
	{
		this->arrayBits.set(i, Qi.arrayBits[i]);
	}

	return *this;
}

// Bitwise
QInt QInt::operator~()
{
	QInt tempQi;
	tempQi.arrayBits = this->arrayBits.flip();
	return tempQi;
}

QInt QInt::operator<<(const uint8_t& n)
{
	QInt temp = *this;
	for (uint8_t i = 0; i < (uint8_t)temp.arrayBits.size(); i++)
	{
		if ((i + n) > (uint8_t)temp.arrayBits.size() - 1)
		{
			temp.arrayBits.set(i, 0);
			continue;
		}
		temp.arrayBits[i] = temp.arrayBits[(long long int)i + n];
	}
	return temp;
}

// Binary Operators
// Arithmetic
QInt QInt::operator+(const QInt& Qi)
{
	bitset<1> temp = { 0 };
	QInt tempQi;
	for (int i = (int)Qi.arrayBits.size() - 1; i >= 0; i--)
	{
		tempQi.arrayBits[i] = (this->arrayBits[i] ^ Qi.arrayBits[i]);
		if (this->arrayBits[i] == Qi.arrayBits[i])
		{
			tempQi.arrayBits[i] = tempQi.arrayBits[i] ^ temp[0];
			if (Qi.arrayBits[i] == 0)
			{
				if (temp[0] == 1)
				{
					temp.flip();
				}
			}
			else
			{
				if (temp[0] == 0)
					temp.flip();
			}
		}
		else
		{
			if (this->arrayBits[i] == 1)
				tempQi.arrayBits[i] = this->arrayBits[i] ^ temp[0];
			else
				tempQi.arrayBits[i] = Qi.arrayBits[i] ^ temp[0];
		}
	}
	return tempQi;
}

QInt QInt::operator-(const QInt& Qi)
{
	QInt tempQi, OneComplement, temp, TwoComplement;
	OneComplement = Qi;
	temp.arrayBits.set(127, 1);
	OneComplement.arrayBits.flip();
	TwoComplement = temp + OneComplement;
	tempQi = *this + TwoComplement;
	return tempQi;
}

QInt QInt::operator*(const QInt& Qi)
 {
	QInt tempQi;
	int countQi = 0;
	for (int i = 0; i < (int)Qi.arrayBits.size(); i++) 
	{
		if (Qi.arrayBits[i] == 1)
			countQi = ((int)Qi.arrayBits.size() - 1) - i;
		if (countQi != 0)
			break;
	}
	QInt temp;
	for (int i = (int)Qi.arrayBits.size() - 1; i >= ((int)Qi.arrayBits.size() - 1) - countQi; i--)
	 {
		if (Qi.arrayBits[i] == 1) 
		{
			temp = *this << ((int)Qi.arrayBits.size() - 1) - i;
		}
		tempQi = tempQi + temp;
	}
	return tempQi;
}

QInt QInt::operator/(const QInt& Qi) 
{
	bool NegaFlag = false;
	QInt tempDivident = *this, tempDivisor = Qi;
	if (this->arrayBits[0] == Qi.arrayBits[0])
	 {
		if (this->arrayBits[0] == 1)
		 {
			tempDivident = tempDivident.toTwoComplement();
			tempDivisor = tempDivisor.toTwoComplement();
		}
	}
	else
	 {
		NegaFlag = true;
		if (this->arrayBits[0] == 1) 
		{
			tempDivident = tempDivident.toTwoComplement();
		}
		else if (Qi.arrayBits[0] == 1) 
		{
			tempDivisor = tempDivisor.toTwoComplement();
		}
	}

	int NumberOfDividentBits = 128;
	int Holder;
	QInt Remainder;
	while (NumberOfDividentBits > 0) 
	{
		Holder = tempDivident.arrayBits[0];
		tempDivident = tempDivident << 1;
		Remainder = Remainder << 1;
		Remainder.arrayBits.set(127, Holder);
		QInt tempPrevRemainder = Remainder;
		Remainder = Remainder - tempDivisor;
		if (Remainder.arrayBits[0] == 0)
		{
			tempDivident.arrayBits.set(127, 1);
		}
		else 
		{
			tempDivident.arrayBits.set(127, 0);
			Remainder = tempPrevRemainder;
		}
		NumberOfDividentBits--;
	}
	if (NegaFlag)
		tempDivident = tempDivident.toTwoComplement();
	return tempDivident;
}

QInt QInt::operator>>(const uint8_t& n)
{
	QInt tempQi = *this;
	int temp = tempQi.arrayBits[0];
	int count = 1;
	int bound = n;
	if (bound > 127)
		bound = 127;
	while (count <= bound)
	{
		for (int i = (int)tempQi.arrayBits.size() - 1; i >= 1; i--)
		{
			tempQi.arrayBits[i] = tempQi.arrayBits[(long long int)i - 1];
		}
		count++;
	}
	for (int i = count - 1; i >= 0; i--)
	{
		tempQi.arrayBits[i] = temp;
	}
	return tempQi;
}

QInt QInt::operator&(const QInt& Qi)
{
	QInt tempQi;
	for (int i = 0; i < (int)Qi.arrayBits.size(); i++)
	{
		tempQi.arrayBits[i] = this->arrayBits[i] & Qi.arrayBits[i];
	}
	return tempQi;
}

QInt QInt::operator|(const QInt& Qi)
{
	QInt tempQi;
	for (int i = 0; i < (int)Qi.arrayBits.size(); i++)
	{
		tempQi.arrayBits[i] = this->arrayBits[i] | Qi.arrayBits[i];
	}
	return tempQi;
}

QInt QInt::ror()
{
	QInt tempQi;
	int carryBit = this->arrayBits[127];
	tempQi = *this >> 1;
	tempQi.arrayBits.set(0, carryBit);
	return tempQi;
}

QInt QInt::rol()
{
	QInt tempQi;
	int carryBit = this->arrayBits[0];
	tempQi = *this << 1;
	tempQi.arrayBits.set(127, carryBit);
	return tempQi;
}

QInt QInt::operator^(const QInt& Qi)
{
	QInt tempQi;
	for (int i = 0; i < (int)Qi.arrayBits.size(); i++)
	{
		tempQi.arrayBits[i] = this->arrayBits[i] ^ Qi.arrayBits[i];
	}
	return tempQi;
}

bool handleFile(string inFile, string outFile)
{
	ifstream fin; fin.open(inFile);
	ofstream fout; fout.open(outFile);

	if (!fin.is_open())
	{
		cout << "Can't open file \"" << inFile << "\" or file \"" << outFile << "\"." << endl
			<< "Please check again!" << endl;
		return false;
	}

	string operatorLine;

	while (!fin.eof())
	{
		getline(fin, operatorLine);

		size_t found = operatorLine.find(" ", 0);
		int count = 1;

		while (found != string::npos)
		{
			found = operatorLine.find(" ", found + 1);
			if (found != string::npos) count++;
		}

		if (count == 2)
		{
			string base, op, value;
			char* nextToken = nullptr;

			char* cstr = new char[operatorLine.length() + 1];
			strcpy(cstr, operatorLine.c_str());

			char* stringToken = strtok_s(cstr, " ", &nextToken);

			while (stringToken != nullptr)
			{
				base.assign(stringToken);
				stringToken = strtok_s(NULL, " ", &nextToken);
				op.assign(stringToken);
				stringToken = strtok_s(NULL, " ", &nextToken);
				value.assign(stringToken);
				stringToken = strtok_s(NULL, " ", &nextToken);
			}

			QInt value_QInt;
			if (base == "10")
			{
				value_QInt.toBase2_10(value);
			}
			else if (base == "16")
			{
				value_QInt.toBase2_16(value);
			}
			else
			{
				value_QInt.toBase2_2(value);
			}

			if (op == "2")
			{
				fout << value_QInt.getBits() << endl;
			}
			else if (op == "10")
			{
				fout << value_QInt.toBase10() << endl;
			}
			else if (op == "16")
			{
				fout << value_QInt.toBase16() << endl;
			}
			else if (op == "~")
			{
				QInt tempQInt = ~value_QInt;
				if (base == "2")
				{
					fout << tempQInt.getBits() << endl;
				}
				else if (base == "10")
				{
					fout << tempQInt.toBase10() << endl;
				}
				else if (base == "16")
				{
					fout << tempQInt.toBase16() << endl;
				}
			}
			else if (op == "rol")
			{
				QInt tempQInt; tempQInt = value_QInt.rol();
				if (base == "2")
				{
					fout << tempQInt.getBits() << endl;
				}
				else if (base == "10")
				{
					fout << tempQInt.toBase10() << endl;
				}
				else if (base == "16")
				{
					fout << tempQInt.toBase16() << endl;
				}
			}
			else if (op == "ror")
			{
				QInt tempQInt; tempQInt = value_QInt.ror();
				if (base == "2")
				{
					fout << tempQInt.getBits() << endl;
				}
				else if (base == "10")
				{
					fout << tempQInt.toBase10() << endl;
				}
				else if (base == "16")
				{
					fout << tempQInt.toBase16() << endl;
				}
			}
			else
			{
				continue;
			}

			delete[]cstr;
		}
		else if (count == 3)
		{
			string base, op, value1, value2;
			char* nextToken = nullptr;

			char* cstr = new char[operatorLine.length() + 1];
			strcpy(cstr, operatorLine.c_str());

			char* stringToken = strtok_s(cstr, " ", &nextToken);

			while (stringToken != nullptr)
			{
				base.assign(stringToken);
				stringToken = strtok_s(NULL, " ", &nextToken);
				value1.assign(stringToken);
				stringToken = strtok_s(NULL, " ", &nextToken);
				op.assign(stringToken);
				stringToken = strtok_s(NULL, " ", &nextToken);
				value2.assign(stringToken);
				stringToken = strtok_s(NULL, " ", &nextToken);
			}

			QInt value1_QInt, value2_QInt;
			if (base == "10")
			{
				value1_QInt.toBase2_10(value1);
				value2_QInt.toBase2_10(value2);
			}
			else if (base == "16")
			{
				value1_QInt.toBase2_16(value1);
				value2_QInt.toBase2_16(value2);
			}
			else
			{
				value1_QInt.toBase2_2(value1);
				value2_QInt.toBase2_2(value2);
			}

			if (op == "+")
			{
				QInt tempQInt = value1_QInt + value2_QInt;
				if (base == "2")
				{
					fout << tempQInt.getBits() << endl;
				}
				else if (base == "10")
				{
					fout << tempQInt.toBase10() << endl;
				}
				else if (base == "16")
				{
					fout << tempQInt.toBase16() << endl;
				}
			}
			else if (op == "-")
			{
				QInt tempQInt = value1_QInt - value2_QInt;
				if (base == "2")
				{
					fout << tempQInt.getBits() << endl;
				}
				else if (base == "10")
				{
					fout << tempQInt.toBase10() << endl;
				}
				else if (base == "16")
				{
					fout << tempQInt.toBase16() << endl;
				}
			}
			else if (op == "*")
			{
				QInt tempQInt = value1_QInt * value2_QInt;
				if (base == "2")
				{
					fout << tempQInt.getBits() << endl;
				}
				else if (base == "10")
				{
					fout << tempQInt.toBase10() << endl;
				}
				else if (base == "16")
				{
					fout << tempQInt.toBase16() << endl;
				}
			}
			else if (op == "/")
			{
				QInt tempQInt = value1_QInt / value2_QInt;
				if (base == "2")
				{
					fout << tempQInt.getBits() << endl;
				}
				else if (base == "10")
				{
					fout << tempQInt.toBase10() << endl;
				}
				else if (base == "16")
				{
					fout << tempQInt.toBase16() << endl;
				}
			}
			else if (op == ">>")
			{
				char* cstrShiftingBit = new char[value2.length() + 1];
				strcpy(cstrShiftingBit, value2.c_str());
				uint8_t shiftingBit = atoi(cstrShiftingBit);

				QInt tempQInt = value1_QInt >> shiftingBit;
				if (base == "2")
				{
					fout << tempQInt.getBits() << endl;
				}
				else if (base == "10")
				{
					fout << tempQInt.toBase10() << endl;
				}
				else if (base == "16")
				{
					fout << tempQInt.toBase16() << endl;
				}

				delete[]cstrShiftingBit;
			}
			else if (op == "<<")
			{
				char* cstrShiftingBit = new char[value2.length() + 1];
				strcpy(cstrShiftingBit, value2.c_str());
				uint8_t shiftingBit = atoi(cstrShiftingBit);

				QInt tempQInt = value1_QInt << shiftingBit;
				if (base == "2")
				{
					fout << tempQInt.getBits() << endl;
				}
				else if (base == "10")
				{
					fout << tempQInt.toBase10() << endl;
				}
				else if (base == "16")
				{
					fout << tempQInt.toBase16() << endl;
				}

				delete[]cstrShiftingBit;
			}
			else if (op == "&")
			{
				QInt tempQInt = value1_QInt & value2_QInt;
				if (base == "2")
				{
					fout << tempQInt.getBits() << endl;
				}
				else if (base == "10")
				{
					fout << tempQInt.toBase10() << endl;
				}
				else if (base == "16")
				{
					fout << tempQInt.toBase16() << endl;
				}
			}
			else if (op == "|")
			{
				QInt tempQInt = value1_QInt | value2_QInt;
				if (base == "2")
				{
					fout << tempQInt.getBits() << endl;
				}
				else if (base == "10")
				{
					fout << tempQInt.toBase10() << endl;
				}
				else if (base == "16")
				{
					fout << tempQInt.toBase16() << endl;
				}
			}
			else if (op == "^")
			{
				QInt tempQInt = value1_QInt ^ value2_QInt;
				if (base == "2")
				{
					fout << tempQInt.getBits() << endl;
				}
				else if (base == "10")
				{
					fout << tempQInt.toBase10() << endl;
				}
				else if (base == "16")
				{
					fout << tempQInt.toBase16() << endl;
				}
			}
			else
			{
				continue;
			}

			delete[]cstr;
		}

		operatorLine.clear();
	}

	fin.close(); fout.close();
	return true;
}