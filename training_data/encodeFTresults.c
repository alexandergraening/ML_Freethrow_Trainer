// This program takes user input from keyboard to quickly
// encode results of a freethrow for a FANN training file

// USAGE: ./encodeftresults <outputfileaname> <start number> <end number>

#include <stdio.h>

int main(int argc, char* argv[])
{
	if (argc != 4) {
		fprintf(stderr, "Check Usage: ./encodeFTresults <ENC RESULTS FILE> <Start freethrow number> <end freethrow number>\n");
		return 1;
	}

	char * resultsFileName;
	resultsFileName = argv[1];
	int startFreeThrowNum, endFreeThrowNum;
	startFreeThrowNum = atoi(argv[2]);
	endFreeThrowNum = atoi(argv[3]);
	FILE* oFile;
	oFile = fopen(argv[1], "w");

	int i;
	char userInput;
	for (i = startFreeThrowNum; i < endFreeThrowNum + 1; i++) {
		printf("Enter result of freethrow %d : ", i);
		scanf(" %c", &userInput);
		if (userInput == 'i')
			fprintf(oFile, "0 1 0\n");
		else if (userInput == 'u')
			fprintf(oFile, "1 0 0\n");
		else if (userInput == 'o')
			fprintf(oFile, "0 0 1\n");
		else
			fprintf(oFile, "ERROR: Please manually fix this line for attempt number %d\n", i);
	}

	fclose(oFile);

	return 0;
}
