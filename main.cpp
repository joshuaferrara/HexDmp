#include <iostream>
#include <stdio.h>
#include <sstream>
#include <iomanip>
#include <cmath>

// Cross-platform compilation requirements
#ifdef __unix
#include <stdlib.h>
#define _CRT_SECURE_NO_DEPRECATE
#endif

using namespace std;

// Converts a decimal value into a hexadecimal value, padding it with a 0 or chopping off extra data if necessary.
string hexVal(int dec) {
	stringstream tmp;
	tmp << hex << dec;
	string str = tmp.str();
	if (str.length() > 2) {
		str = str.substr(str.length() - 2);
	}
	else if (str.length() == 1) {
		str = "0" + str;
	}
	return str;
}

// Draws a horizontal line to the output of equal length to the width of the table
void hr() {
	std::cout << std::string(75, '-') << std::endl;
}

int main(int argc, char ** argv) {
	if (argc <= 1) {
		cout << "No file provided!" << endl;
	}
	else {
		FILE *fileptr;
		char *buffer;
		long filelen;

		string currentFile; 
		for (int i = 1; i < argc; i++) { // For each file provided as an argument
			currentFile = argv[i];

			fileptr = fopen(currentFile.c_str(), "rb"); // Create a file handle

			if (fileptr == NULL) { // Check if we can open the file for reading
				cout << "File could not be opened: " << currentFile << endl;
				continue;
			}

			fseek(fileptr, 0, SEEK_END); // Seek to the end of the file
			filelen = ftell(fileptr); // Determine the length of the file
			rewind(fileptr); // Seek to the beginning of the file

			buffer = (char *)malloc((filelen + 1) * sizeof(char)); // Allocate memory space with the size of the file we are reading
			fread(buffer, filelen, 1, fileptr); // Read file into buffer
			fclose(fileptr); // Close file handle

			int rows = ceil(filelen / 16) + 1; // Calculate how many rows we are going to print. Each row is 16 bytes in width.

			hr();
			printf("|%-8s|%-47s|%-16s|\n", "Offset", "Hex Dump", "Text Dump"); // Render header
			hr();

			for (int i = 0, offset = i * 16; i < rows; i++) { // For each section of 16 bytes in the file
				offset = i * 16; // Offset equals current row * 16

				stringstream hexStream;
				stringstream charStream;
				for (int x = 0; x < 16 && (offset + x < filelen - 1); x++) { // For each byte (up until the 16th from the current offset) and not past the end of the file...
					hexStream  << hexVal((int)buffer[offset + x]) << (x == 15 ? "" : " ");
					charStream << (isprint(buffer[offset + x]) ? buffer[offset + x] : '.');
				}

				printf("|%08x|%-47s|%-16s|\n", offset, hexStream.str().c_str(), charStream.str().c_str());
			}

			hr();

			free(buffer); // Free memory
		}
	}

#ifdef _DEBUG
	system("pause");
#endif
}