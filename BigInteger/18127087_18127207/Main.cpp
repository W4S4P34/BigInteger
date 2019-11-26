#include "QInt.h"

int main(int argc, char* argv[])
{
	if (argc == 3)
	{
		string inFile, outFile;
		if (argv[1] != nullptr) inFile.assign(argv[1]);
		if (argv[2] != nullptr) outFile.assign(argv[2]);
		if (handleFile(inFile, outFile))
			cout << "************************" << endl
			<< "*Successfully calculate*" << endl
			<< "************************" << endl;
		else
			cout << "**************************" << endl
			<< "*Unsuccessfully calculate*" << endl
			<< "**************************" << endl;
	}
	else if (argc < 3) 
	{
		cout << "*******************" << endl
			<< "*Too few arguments*" << endl
			<< "*******************" << endl;
	}
	else
	{
		cout << "*********************" << endl
			<< "*Too much arguments!*" << endl
			<< "*********************" << endl;
	}

	return 0;
}