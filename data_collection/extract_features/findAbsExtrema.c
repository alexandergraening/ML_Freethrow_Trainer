/*
 *      This program will open a mdo csv file and find the
 *	times and amplitudes of the absolute max and min
 *	of both the accelerometer and gyroscope.
 *
 *	Then, it will find ratios between maxes of the axes
 *
 *	Then, it writes these these features to output files
 * 
 *	Usage:
 *	./findAbsExtrema <INPUT_FILE> <OUTPUT_FILE_ACC> <OUTPUT_FILE_GYR>
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define BUFF_SIZE 1024
 
void norm_vector(float vector[], float *norm, int n)
{
        int i;
        for (i = 0; i < n; i++){
		vector[i] = vector[i]/(*norm);
        }

}

void max_vector(float vector[], float timeVector[], float *max, float* maxTime, int n)
{
        int i;
	*max = vector[0];	
        for (i = 1; i < n; i++){
		if (*max < vector[i]){
			*max = vector[i];
			*maxTime = timeVector[i];
		}
        }

}

void min_vector(float vector[], float timeVector[], float *min, float* minTime, int n)
{	
       	int i;
        *min = vector[0];
        for (i = 1; i < n; i++){
                if (*min > vector[i]){
                        *min = vector[i];
			*minTime = timeVector[i];
                }
        }
}

int main(int argc, char **argv)
{
	FILE* iFile;
	FILE* oFileAcc;
	FILE* oFileGyr;
        char* ifile_name;
	char* ofile_name_acc;
	char* ofile_name_gyr;
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        int N_SAMPLES;

	int vector_length, rv, i;
	float norm, vec_min, vec_max;
	float minTime;
	float maxTime;
	float * time_vector;
	float * amplitude_vector_acc_X;
	float * amplitude_vector_acc_Y;
	float * amplitude_vector_acc_Z;
	float * amplitude_vector_gyr_X;
	float * amplitude_vector_gyr_Y;
	float * amplitude_vector_gyr_Z;


	              if (argc != 4) {
                       fprintf(stderr, 
                              "Error - check usage\n"
                              );
                        exit(EXIT_FAILURE);
                }


	ifile_name = argv[1];
        ofile_name_acc = argv[2];
	ofile_name_gyr = argv[3];

        /* open the input file */
        //printf(" Input file \'%s\'.\n", ifile_name);
        iFile = fopen(ifile_name, "r");
        if (iFile == NULL) {
                fprintf(stderr, 
                                "Failed to read from file \'%s\'.\n", 
                                ifile_name
                       );
                exit(EXIT_FAILURE);
        }

        /* count the number of lines in the file */
        read = getline(&line, &len, iFile); //discard header of file
        N_SAMPLES = 0;
        while ((read = getline(&line, &len, iFile)) != -1) {
                N_SAMPLES++;
        }

        /* go back to the start of the file so that the data can be read */
        rewind(iFile);
        read = getline(&line, &len, iFile); //discard header of file

	/* decrement N_SAMPLES to account for header line */

	N_SAMPLES--;

        i = 0;

        time_vector = (float *) malloc(sizeof(float) * N_SAMPLES);
        amplitude_vector_acc_X  = (float *) malloc(sizeof(float) * N_SAMPLES);
        amplitude_vector_acc_Y  = (float *) malloc(sizeof(float) * N_SAMPLES);
        amplitude_vector_acc_Z  = (float *) malloc(sizeof(float) * N_SAMPLES);
        amplitude_vector_gyr_X  = (float *) malloc(sizeof(float) * N_SAMPLES);
	amplitude_vector_gyr_Y  = (float *) malloc(sizeof(float) * N_SAMPLES);
	amplitude_vector_gyr_Z  = (float *) malloc(sizeof(float) * N_SAMPLES);

	while ((read = getline(&line, &len, iFile)) != -1) {
                /* parse the data */
                rv = sscanf(line, "%f,%f,%f,%f,%f,%f,%f\n", &time_vector[i], &amplitude_vector_acc_X[i], 
		&amplitude_vector_acc_Y[i], &amplitude_vector_acc_Z[i], 
		&amplitude_vector_gyr_X[i], &amplitude_vector_gyr_Y[i], &amplitude_vector_gyr_Z[i]);
                i++;
        }
        fclose(iFile);
	vector_length = N_SAMPLES;

	oFileAcc = fopen(ofile_name_acc, "w");

	min_vector(amplitude_vector_acc_X, time_vector, &vec_min,&minTime, vector_length);
	max_vector(amplitude_vector_acc_X, time_vector, &vec_max,&maxTime, vector_length);
	float minX = vec_min; float minTimeX = minTime;
	float maxX = vec_max; float maxTimeX = maxTime;
	min_vector(amplitude_vector_acc_Y, time_vector, &vec_min,&minTime, vector_length);
	max_vector(amplitude_vector_acc_Y, time_vector, &vec_max,&maxTime, vector_length);
	float minY = vec_min; float minTimeY = minTime;
	float maxY = vec_max; float maxTimeY = maxTime;
	min_vector(amplitude_vector_acc_Z, time_vector, &vec_min,&minTime, vector_length);
	max_vector(amplitude_vector_acc_Z, time_vector, &vec_max,&maxTime, vector_length);
	float minZ = vec_min; float minTimeZ = minTime;
	float maxZ = vec_max; float maxTimeZ = maxTime;

	/*printf("Accelerometer\n");
	printf("X Minimum: %f at time: %f	X Maximum: %f at time %f\n", minX, minTimeX, maxX, maxTimeX);
	printf("Y Minimum: %f at time: %f	Y Maximum: %f at time %f\n", minY, minTimeY, maxY, maxTimeY);
	printf("Z Minimum: %f at time: %f	Z Maximum: %f at time %f\n", minZ, minTimeZ, maxZ, maxTimeZ);*/
	
	fprintf(oFileAcc, "time,xMin,time,xMax,time,yMin,time,yMax,time,zMin,time,zMax (Accelerometer)\n");
	fprintf(oFileAcc, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",minTimeX,minX,maxTimeX,maxX,minTimeY,minY,maxTimeY,maxY,minTimeZ,minZ,maxTimeZ,maxZ);
	fprintf(oFileAcc, "Time Features (x,y,z):\n%f,%f,%f\n", minTimeX-maxTimeX, minTimeY-maxTimeY, minTimeZ-maxTimeZ);
	fprintf(oFileAcc, "Ratio of Maximums (XY, YZ, ZX):\n%f, %f, %f\n", maxX/maxY, maxY/maxZ, maxZ/maxX);	
	fclose(oFileAcc);

	oFileGyr = fopen(ofile_name_gyr, "w");
	
	min_vector(amplitude_vector_gyr_X, time_vector, &vec_min,&minTime, vector_length);
	max_vector(amplitude_vector_gyr_X, time_vector, &vec_max,&maxTime, vector_length);
	minX = vec_min; minTimeX = minTime;
	maxX = vec_max; maxTimeX = maxTime;
	min_vector(amplitude_vector_gyr_Y, time_vector, &vec_min,&minTime, vector_length);
	max_vector(amplitude_vector_gyr_Y, time_vector, &vec_max,&maxTime, vector_length);
	minY = vec_min; minTimeY = minTime;
	maxY = vec_max; maxTimeY = maxTime;
	min_vector(amplitude_vector_gyr_Z, time_vector, &vec_min,&minTime, vector_length);
	max_vector(amplitude_vector_gyr_Z, time_vector, &vec_max,&maxTime, vector_length);
	minZ = vec_min; minTimeZ = minTime;
	maxZ = vec_max; maxTimeZ = maxTime;
	/*printf("Gyroscope\n");
	printf("X Minimum: %f at time: %f	X Maximum: %f at time %f\n", minX, minTimeX, maxX, maxTimeX);
	printf("Y Minimum: %f at time: %f	Y Maximum: %f at time %f\n", minY, minTimeY, maxY, maxTimeY);
	printf("Z Minimum: %f at time: %f	Z Maximum: %f at time %f\n", minZ, minTimeZ, maxZ, maxTimeZ);*/
	
	fprintf(oFileGyr, "time,xMin,time,xMax,time,yMin,time,yMax,time,zMin,time,zMax (Gyroscope)\n");
	fprintf(oFileGyr, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",minTimeX,minX,maxTimeX,maxX,minTimeY,minY,maxTimeY,maxY,minTimeZ,minZ,maxTimeZ,maxZ);
	fprintf(oFileGyr, "Time Features (x,y,z):\n%f,%f,%f\n", minTimeX-maxTimeX, minTimeY-maxTimeY, minTimeZ-maxTimeZ);	
	fprintf(oFileAcc, "Ratio of Maximums (XY, YZ, ZX):\n%f, %f, %f\n", maxX/maxY, maxY/maxZ, maxZ/maxX);	
	fclose(oFileGyr);

    	return 0;
}

