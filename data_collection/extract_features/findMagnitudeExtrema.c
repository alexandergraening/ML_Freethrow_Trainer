/*
 *      This program will open a magnitude csv file and find the
 *	times and amplitudes of the absolute max and min.
 *	
 *	Then it will find the time difference between max/min
 *	Then, it writes these these features to output file.
 *
 *
 * 
 *	Usage:
 *	./findMagExtrema <INPUT_FILE> <OUTPUT_FILE>
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define BUFF_SIZE 1024

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
	FILE* oFile;
        char* ifile_name;
	char* ofile_name;
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        int N_SAMPLES;

	int vector_length, rv, i;
	float norm, vec_min, vec_max;
	float minTime;
	float maxTime;
	float deltaT;
	float * time_vector;
	float * amplitude_vector;

	if (argc != 3) {
                       fprintf(stderr, 
                              "Error - check usage\n"
                              );
                        exit(EXIT_FAILURE);
        }

	ifile_name = argv[1];
        ofile_name = argv[2];

        /* open the input file */
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
        amplitude_vector = (float *) malloc(sizeof(float) * N_SAMPLES);

	while ((read = getline(&line, &len, iFile)) != -1) {
                /* parse the data */
                rv = sscanf(line, "%f,%f,%f,%f,%f,%f,%f\n", &time_vector[i], &amplitude_vector[i]);
                i++;
        }
        fclose(iFile);
	vector_length = N_SAMPLES;

	oFile = fopen(ofile_name, "w");
	if (iFile == NULL) {
		fprintf(stderr, "Failed to open output file\n");
		exit(EXIT_FAILURE);
	}

	min_vector(amplitude_vector, time_vector, &vec_min,&minTime, vector_length);
	max_vector(amplitude_vector, time_vector, &vec_max,&maxTime, vector_length);
	deltaT = minTime - maxTime;
	fprintf(oFile, "time,Min,time,Max,deltaT\n");
	fprintf(oFile, "%f,%f,%f,%f,%f\n", minTime, vec_min, maxTime, vec_max, deltaT);
	
	fclose(oFile);

    	return 0;
}

