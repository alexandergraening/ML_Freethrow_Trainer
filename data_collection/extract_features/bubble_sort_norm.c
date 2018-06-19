/*
 *      Perform bubble sort on two vector pairs where
 *      first vector, time_vector value determine
 *      sort order from minimum to maximum value
 *
 *	Time values are normalized to difference between 
 *	maximum and minimum values
 *
 *      Amplitude values are normalized to difference between 
 *      maximum and minimum values
 *
 *	Usage:
 *	./bubble_sort_norm <INPUT_FILE1> <INPUT_FILE2> <OUTPUT_FILE1> <Outputfile2>
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define BUFF_SIZE 1024
 
void exchange(float *vec1, float *vec2)
{
    	float tmp = *vec1;
    	*vec1 = *vec2;
    	*vec2 = tmp;
}

void bubble_sort(float vector1[], float vector2[], int n)
{
   	int i, j;
   	for (i = 0; i < n-1; i++){
       		for (j = 0; j < n-i-1; j++){
           	if (vector1[j] > vector1[j+1]){
              		exchange(&vector1[j], &vector1[j+1]);
			exchange(&vector2[j], &vector2[j+1]);
			}
		}
	}
	
}

void norm_vector(float vector[], float *norm, int n)
{
        int i;
        for (i = 0; i < n; i++){
		vector[i] = vector[i]/(*norm);
        }

}

void max_vector(float vector[], float *max, int n)
{
        int i;
	*max = vector[0];	
        for (i = 1; i < n; i++){
		if (*max < vector[i]){
			*max = vector[i];
		}
        }

}

void min_vector(float vector[], float *min, int n)
{	
       	int i;
        *min = vector[0];
        for (i = 1; i < n; i++){
                if (*min > vector[i]){
                        *min = vector[i];
                }
        }
}

int main(int argc, char **argv)
{
	FILE *fp;
        char *ifile_name, *ofile_name;
	char *ifile_name2, *ofile_name2;
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        int N_SAMPLES;
	int N_SAMPLES2;
	int vector_length, rv, i;
	float norm, vec_min, vec_max;

	float * time_vector;
	float * amplitude_vector;
	float * time_vector2;
	float * amplitude_vector2;

	              if (argc != 5) {
                       fprintf(stderr, 
                              "Error - check usage\n"
                              );
                        exit(EXIT_FAILURE);
                }


	ifile_name = argv[1];
	ifile_name2 = argv[2];
        ofile_name = argv[3];
	ofile_name2 = argv[4];
        /* open the 1st input file */
        printf(" Input file \'%s\'.\n", ifile_name);
        fp = fopen(ifile_name, "r");
        if (fp == NULL) {
                fprintf(stderr, 
                                "Failed to read from file \'%s\'.\n", 
                                ifile_name
                       );
                exit(EXIT_FAILURE);
        }

        /* count the number of lines in the file */
        read = getline(&line, &len, fp); //discard header of file
        N_SAMPLES = 0;
        while ((read = getline(&line, &len, fp)) != -1) {
                N_SAMPLES++;
        }

        /* go back to the start of the file so that the data can be read */
        rewind(fp);
        read = getline(&line, &len, fp); //discard header of file

	N_SAMPLES--;

	/* decrement N_SAMPLES to account for header line */

        i = 0;
        time_vector = (float *) malloc(sizeof(float) * N_SAMPLES);
        amplitude_vector  = (float *) malloc(sizeof(float) * N_SAMPLES);
        while ((read = getline(&line, &len, fp)) != -1) {
                /* parse the data */
                rv = sscanf(line, "%f,%f\n", &time_vector[i], &amplitude_vector[i]);
                if (rv != 2) {
                        fprintf(stderr,
                                        "%s %d \'%s\'\n",
                                        "Skip line",
                                        i,
                                        line
                               );
		continue;
                }
                i++;
        }
        fclose(fp);

        /* open the 2nd input file */
        printf(" Input file \'%s\'.\n", ifile_name2);
        fp = fopen(ifile_name2, "r");
        if (fp == NULL) {
                fprintf(stderr, 
                                "Failed to read from file \'%s\'.\n", 
                                ifile_name2
                       );
                exit(EXIT_FAILURE);
        }

        /* count the number of lines in the file */
        read = getline(&line, &len, fp); //discard header of file
        N_SAMPLES2 = 0;
        while ((read = getline(&line, &len, fp)) != -1) {
                N_SAMPLES2++;
        }

        /* go back to the start of the file so that the data can be read */
        rewind(fp);
        read = getline(&line, &len, fp); //discard header of file

	/* decrement N_SAMPLES to account for header line */

        i = 0;
        time_vector2 = (float *) malloc(sizeof(float) * N_SAMPLES2);
        amplitude_vector2  = (float *) malloc(sizeof(float) * N_SAMPLES2);
        while ((read = getline(&line, &len, fp)) != -1) {
                /* parse the data */
                rv = sscanf(line, "%f,%f\n", &time_vector2[i], &amplitude_vector2[i]);
                if (rv != 2) {
                        fprintf(stderr,
                                        "%s %d \'%s\'\n",
                                        "Skip line",
                                        i,
                                        line
                               );
		continue;
                }
                i++;
        }
        fclose(fp);

	vector_length = N_SAMPLES + N_SAMPLES2;
	float* time_vector3 = (float *) malloc(sizeof(float) * vector_length);	
	float* amplitude_vector3 = (float *) malloc(sizeof(float) * vector_length);
	memcpy(time_vector3, time_vector, N_SAMPLES * sizeof(float));
	memcpy(time_vector3 + N_SAMPLES, time_vector2, N_SAMPLES2 *sizeof(float));
	memcpy(amplitude_vector3, amplitude_vector, N_SAMPLES * sizeof(float));
	memcpy(amplitude_vector3 + N_SAMPLES, amplitude_vector2, N_SAMPLES2 *sizeof(float));
	
	bubble_sort(time_vector, amplitude_vector, N_SAMPLES);
	bubble_sort(time_vector2, amplitude_vector2, N_SAMPLES2);    	
	bubble_sort(time_vector3, amplitude_vector3, vector_length);
/*
	printf("Time Vector Values\n");
    	for (i=0; i < vector_length; i++){
        	printf("%f\n", time_vector3[i]);
	}

	printf("Amplitude Vector Values\n");

	for (i=0; i < vector_length; i++){
                printf("%f\n", amplitude_vector3[i]);
        }
*/

	// Normalize the time vectors
	norm = time_vector3[vector_length-1];
	norm_vector(time_vector3, &norm, vector_length);

	norm_vector(time_vector, &norm, N_SAMPLES);
	norm_vector(time_vector2, &norm, N_SAMPLES2);

/*
        printf("Time Vector Values Normalized\n");
        for (i=0; i < vector_length; i++){
                printf("%f\n", time_vector3[i]);
        }
*/

	// Normalize the amp vectors
	min_vector(amplitude_vector3, &vec_min, vector_length);
	max_vector(amplitude_vector3, &vec_max, vector_length);
	printf("min %f max %f\n", vec_min, vec_max);
	norm = vec_max - vec_min;
	norm_vector(amplitude_vector3, &norm, vector_length);
  	norm_vector(amplitude_vector, &norm, N_SAMPLES);
	norm_vector(amplitude_vector2, &norm, N_SAMPLES2);     
       /*
        printf("Amplitude Vector Values Normalized\n");
        for (i=0; i < vector_length; i++){
                printf("%f\n", amplitude_vector3[i]);
        }
*/
        /* open the output file to write the x axis data */
        
	printf("Sorted and normalized data file \'%s\'.\n", ofile_name);
        fp = fopen(ofile_name, "w");
        if (fp == NULL) {
                fprintf(stderr, 
                                "Failed to write to file \'%s\'.\n", 
                                ofile_name
                       );
                exit(EXIT_FAILURE);
        }

	/* Write output data at offset 1 to ignore zero entry */

        fprintf(fp, "Normalized Time and Amplitude\n");
        for (i = 0; i < N_SAMPLES; i++) {
                fprintf(fp, "%f, %f\n",
                                time_vector[i],
                                amplitude_vector[i]
                       );
        }

	fclose(fp);
	
	// Open the output file to write the y axis data
	fp = fopen(ofile_name2, "w");
        fprintf(fp, "Normalized Time and Amplitude\n");
        for (i = 0; i < N_SAMPLES2; i++) {
                fprintf(fp, "%f, %f\n",
                                time_vector2[i],
                                amplitude_vector2[i]
                       );
        }
        fclose(fp);

    	return 0;
}

