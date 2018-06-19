// USAGE: ./accGyrMag <input csv> <acceleration output csv> <gyroscope output csv>

//Note: I made changes to calculate gyro magnitude and put max and mins in a different file as well. - Alex

#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		printf("check usage\n");
		return 1;
	}
	FILE* inFile;
	FILE* outFileAcc;
	FILE* outFileGyr;
	
	float timestamp, accX, accY, accZ, accMag, gyrX, gyrY, gyrZ, gyrMag;

	// the following is a dummy variable used only to skip lines when reading files
	char full_line_storage[999];	
	
	inFile = fopen(argv[1],"r");
	if(!inFile)
	{
		printf("FAILURE TO FIND MOTION DATA OUTPUT FILE\n");
		printf("ABORTING\n");
		return 1;
	}
	outFileAcc = fopen(argv[2], "w");
	outFileGyr = fopen(argv[3], "w");
	if(outFileAcc == NULL || outFileGyr == NULL)
	{
		printf("Failure to open output files! Exiting\n");
		return 1;
	}
	fprintf(outFileAcc, "Timestamp    Acc Mags\n");
	fprintf(outFileGyr, "Timestamp    Gyr Mags\n");

	// skip the first header line when reading the csv
	while(fgets(full_line_storage, 999, inFile)!=NULL)
	{
		fscanf(inFile, "%f,%f,%f,%f,%f,%f,%f", &timestamp, &accX, &accY, &accZ, &gyrX, &gyrY, &gyrZ);
		accMag = sqrt(accX*accX + accY*accY + accZ*accZ);
		gyrMag = sqrt(gyrX*gyrX + gyrY*gyrY + gyrZ*gyrZ);
		fprintf(outFileAcc, "%f, %f\n",timestamp, accMag);
		fprintf(outFileGyr, "%f, %f\n",timestamp, gyrMag);
	}

	fclose(outFileAcc);
	fclose(outFileGyr);
	fclose(inFile);
	return 0;
}
