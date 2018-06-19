// This program truncates SensorTile Outputs to remove
// garbage data acquired after the freethrow motion.
// The cutoff point is 2 seconds after the freethrow motion
// found by examining peaks of the ACC Magnitude csv

// USAGE: ./computeRiemannSum <mdo_INPUT> <OUTPUT_FILE>

#include <stdio.h>
#include <stdlib.h>



// Find the maximum peak within the freethrow motion and cutoff time for truncation
void max_vector(float vector[], float* max, int n, float timeVector[], float* cutoffTime)
{

	int i;
	*max = -999;
	
	for (i = 0; i < n; i++) {
		if ( *max < vector[i]){
			*max = vector[i];
			*cutoffTime = timeVector[i]+1;
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Check Usage of computeRiemannSum.c\nCorrect usage: ./computeRiemannSum <input_mdo> <output_file>\n");
		return 1;
	}
	FILE* iFile;
	FILE* oFile;
	char* inputFileName = argv[1]; // main file input
	char* outputFileName = argv[2]; // secondary file input to find time of peak
	char* line = NULL;
	ssize_t read;
	size_t len = 0;
	int lineCount = 0;
	
	// storage for values in Peak File
	float *timeVec;
	float *axVec;
	float *ayVec;
	float *azVec;
	float *gxVec;
	float *gyVec;
	float *gzVec;
	float *mxVec;
	float *myVec;
	float *mzVec;
	int rv; 	// count number of variables read with sscanf

	float maxPeak;
	float cutOff;	

	// Count the number of lines in the Input File
	iFile = fopen(inputFileName, "r");
	if (iFile == NULL) {
		fprintf(stderr, "computeRiemannSum: failed to read from input_file\n");
		exit(EXIT_FAILURE);
	}
	oFile = fopen(outputFileName, "w");
	if (oFile == NULL) {
		fprintf(stderr, "truncData: failed to open output file\n");
		exit(EXIT_FAILURE);
	}

	read = getline(&line, &len, iFile); // disregard header of file
	while((read = getline(&line, &len, iFile)) != -1)
		lineCount++;
	
	rewind(iFile); // back to top
	read = getline(&line, &len, iFile); // disregard header of file
	timeVec = (float*) malloc(sizeof(float) * lineCount);
	axVec = (float*) malloc(sizeof(float) * lineCount);
	ayVec = (float*) malloc(sizeof(float) * lineCount);
	azVec = (float*) malloc(sizeof(float) * lineCount);
	gxVec = (float*) malloc(sizeof(float) * lineCount);
	gyVec = (float*) malloc(sizeof(float) * lineCount);
	gzVec = (float*) malloc(sizeof(float) * lineCount);
	mxVec = (float*) malloc(sizeof(float) * lineCount);
	myVec = (float*) malloc(sizeof(float) * lineCount);
	mzVec = (float*) malloc(sizeof(float) * lineCount);

	int i;	
	read = getline(&line, &len, iFile); // disregard header of file
	i = 0;		
	while((read = getline(&line, &len, iFile)) != -1 && i < lineCount) 
	{
		rv = sscanf(line, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", &timeVec[i], &axVec[i], &ayVec[i], &azVec[i], &gxVec[i], &gyVec[i], &gzVec[i], &mxVec[i], &myVec[i], &mzVec[i]);
		++i;
	}

	double axSum = 0;
	double aySum = 0;
	double azSum = 0;
	double gxSum = 0;
	double gySum = 0;
	double gzSum = 0;
	i = 0;
	while(i < lineCount)
	{
		axSum+=axVec[i];
		aySum+=ayVec[i];
		azSum+=azVec[i];
		gxSum+=gxVec[i];
		gySum+=gyVec[i];
		gzSum+=gzVec[i];
		++i;
	}
	double axAve = axSum/lineCount;
	double ayAve = aySum/lineCount;
	double azAve = azSum/lineCount;
	double gxAve = gxSum/lineCount;
	double gyAve = gySum/lineCount;
	double gzAve = gzSum/lineCount;
	//outVal[] Holds normalized values for FANN training.
	double outVal[6];
	outVal[0] = axAve/1000;
	outVal[1] = ayAve/1000;
	outVal[2] = azAve/1000;
	outVal[3] = gxAve/1000;
	outVal[4] = gyAve/1000;
	outVal[5] = gzAve/1000;

	//fprintf(oFile, "%f\t%f\t%f\t%f\t%f\t%f\t\n",axAve,ayAve,azAve,gxAve,gyAve,gzAve);
	fprintf(oFile, "%f\t%f\t%f\t%f\t%f\t%f\t\n",outVal[0],outVal[1],outVal[2],outVal[3],outVal[4],outVal[5]);

	free (timeVec);
	free (axVec);
	free (ayVec);
	free (azVec);
	free (gxVec);
	free (gyVec);
	free (gzVec);
	free (mxVec);
	free (myVec);
	free (mzVec);

	fclose(iFile);
	fclose(oFile);

	return 0;
}

