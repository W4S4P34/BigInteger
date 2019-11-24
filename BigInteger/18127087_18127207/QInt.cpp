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

// Base Conversion Methods
void QInt::toBase2_10(string base)
{
	if (base == "0") return;
	else
	{
		// Erase the minus "-"
		if (base[0] == '-')
		{
			this->arrayBits.set(0);
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
			tempString = convertOneHexToBin(base[((int)base.length() - 1) - (31 - (i / 4))]);
			for (int j = i; j <= i + 3; j++)
			{
				mode = (tempString[j - i] == '1');
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
		if (this->arrayBits[127 - i])
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

		while (power != 0)
		{
			stringIndex = 38;
			while (stringIndex != 0)
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
	else if (chunk == "1110") return "F";
	else if (chunk == "1111") return "G";
	else return "0";
}

/*************************************************************************/
