// USAGE: ./createTrainFile <FEATURES FILE> <RESULTS FILE> <OUTPUT FILE>

#include <stdio.h>

#define NUM_FEATURES 12
#define NUM_CLASSIFICATIONS 3

int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		printf("Check createTrainFile usage: ./createTrainFile <Features File> <Results Encoding File> <Output Fann Train File> \n");
		return 1;
	}

	printf("Beginning Training File Compilation... \n");
	FILE* outFile;				// For writing formatted FANN training file
	FILE* inFile; 				// For reading features
	FILE* inFile2;				// For reading freethrow results

	
	float features[NUM_FEATURES];
	
	char* buffer;				// Store an entire line of result file
	size_t len = 0;	
	int numFreeThrows;
	int numFeatures;
	inFile = fopen(argv[1], "r");
	inFile2 = fopen(argv[2], "r");
	outFile = fopen(argv[3], "w");
		
	
	printf("Enter the number of training lines to add to the system: ");
	scanf("%d", &numFreeThrows);


	// Write the header file for FANN training file
	fprintf(outFile, "%d %d %d\n", numFreeThrows, NUM_FEATURES, NUM_CLASSIFICATIONS);	
	
	// Read and Write the Files
	int j;
	for(j = 0; j < numFreeThrows; j++)
	{
		int r;
		// Read the features
		if (getline(&buffer, &len, inFile) == -1) {
			printf("Feature File EOF reached... Exiting\n");
			return 1;
		}
		
		// Write the features
		fprintf(outFile, "%s", buffer);

		// Read the result encoding
		if (getline(&buffer, &len, inFile2) == -1) {
			printf("Result File EOF reached... Exiting");
			return 1;
		}	
		// Write the result
		fprintf(outFile, "%s", buffer);
		// Discard the empty line	
		if (getline(&buffer, &len, inFile2) == -1) {
			printf("Result File EOF reached... Exiting");
			return 1;
		}	
	}

	fclose(inFile);
	fclose(inFile2);
	fclose(outFile);	
	return 0;
}
