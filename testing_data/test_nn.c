// This program takes as input a list of features from Sensor Tile Data
// and predicts the outcome of the freethrow motion
// Then it stores the predictions in an output file

// USAGE: ./test_nn <formatted testing file> <output_file>


#include <unistd.h>
#include <stdio.h>
#include "fann.h"

#define N_FEATURES 16

int main(int argc, char* argv [])
{
	if (argc != 2) {
		fprintf(stderr, "Check usage: ./test_nn <fann test file>\n");
		return 1;
	}

	// File opening/reading variables
	FILE* iFile;
	char * iFileName;	
	iFileName = argv[1];
	iFile = fopen(iFileName, "r");
	if(!iFile) {
		fprintf(stderr, "./Fann_test failed to open file: %s\n", iFileName);
		return 1;
	}
	char* line = NULL;
	size_t len = 0;
	ssize_t read;
	int N_SAMPLES;

	// N_SAMPLES x N_FEATURES 2D array of features
	double* features;

	// count number of lines in the file and allocate enough space to store them
	N_SAMPLES = 0;
	while ((read = getline(&line, &len, iFile)) != -1) {
		N_SAMPLES++;
	}

	features = (double *) malloc(sizeof(double) * N_SAMPLES * N_FEATURES);
	// index into the 2d array with features[feature_num + sample_num * N_FEATURES]
	
	rewind(iFile);
	
	int j, k;
	for (j = 0; j < N_SAMPLES; j++) {
		for( k = 0; k < N_FEATURES - 1; k++) 
			fscanf(iFile, "%lf ", &features[k + j * N_FEATURES]);
		fscanf(iFile, "%lf\n", &features[N_FEATURES-1 + j * N_FEATURES]);
	}

/*
	for (j = 0; j < N_SAMPLES; j++) {
		printf("Trial %d Features: " , j+1);
		for (k = 0; k < N_FEATURES; k++) {
			printf("%lf ", features[k + j*N_FEATURES]);
		}
		printf("\n");
	}

	
	// testing if this wrote correctly... 
	for (j = 0; j < N_SAMPLES * N_FEATURES; j++)
		printf("index %d is %f\n", j, features[j]);

*/
	
	// Input the features into the Neural Network
    	struct fann *ann;
    	ann = fann_create_from_file("../training_data/TEST.net");
	fann_type *calc_out;
	fann_type *inputs;
	inputs = (fann_type *) malloc(sizeof(fann_type) * N_FEATURES);
	for (j = 0; j < N_SAMPLES; j++) {
		for (k = 0; k < N_FEATURES; k++) {
			inputs[k] = features[k + j * N_FEATURES];
		//	printf("Inputs is %lf\n", features[k+j*N_FEATURES]);
		}
        	calc_out = fann_run(ann, inputs);


		// Predict the Outcome of the Freethrow
		int i, location;
    		float max;
       		max = -999;
        	for (i = 0; i < 3; i++) {
		//	printf("for location %d: probability is: %f\n" , i, calc_out[i]);
           		 if (calc_out[i] > max) {
               			 max = calc_out[i];
               			 location = i;
           		 }
       		}
		if (location == 0)
        		printf("Trial %d result: UNDER\n", j+1);
	
		if (location == 1)
        		printf("Trial %d result: IN\n", j+1);
	
		if (location == 2)
        		printf("Trial %d result: OVER\n", j+1);
	
	
	}

	free(features);
	free(inputs);
	fclose(iFile);
        sleep(1);

    
    	fann_destroy(ann);


	
    return 0;
}
