// USAGE: ./createTrainFile <OUTPUT FILE> <Trial Number>


// TODO: Create a header line with number of inputs, outputs, training sets

#include <stdio.h>

int main(int argc, char* argv[])
{
	if((argc != 3) || (atoi(argv[2]) == 0))
	{
		printf("Check usage! \n");
		return 1;
	}

	FILE* outFile;
	char userInput[60];
	int trialNum;
	outFile = fopen(argv[1], "a+");
	trialNum = atoi(argv[2]);

	ssize_t read;
	char* line = NULL;
	size_t len = 0;

	printf("Freethrow Result Encoding:\n");
	printf("r - RIGHT\n");
	printf("i - IN\n");
	printf("l - LEFT\n");
	printf("u - SHORT\n");
	printf("o - LONG\n");
	printf("E - ERROR)\n");
	printf("Enter the Result Encoding: ");
	scanf(" %c", &userInput);

	// clear bufer in case user enters too many chars
	while ((getchar() != '\n'));


	fprintf(outFile, "\n");	

	if(userInput[0] == 'r')
		fprintf(outFile, "0 0 0 0 1\n", outFile);
		
	else if (userInput[0] == 'l')
		fprintf(outFile, "0 0 0 1 0\n", outFile);

	else if (userInput[0] == 'i')
		fprintf(outFile, "0 0 1 0 0\n", outFile);

	else if (userInput[0] == 'u')
		fprintf(outFile, "0 1 0 0 0\n", outFile);

	else if (userInput[0] == 'o')
		fprintf(outFile, "1 0 0 0 0\n", outFile);

	else 
		fprintf(outFile, "FEATURES OF TRIAL %d NOT WRITTEN\n", trialNum);


	fclose(outFile);	
	return 0;
}
