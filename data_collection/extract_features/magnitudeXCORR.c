/*
 * Waveform cross correlation FOR MAGNITUDE FILES ONLY (2 column csv)
 * 
 * Input files are the output of Acquire_LowPass_Continuous.c
 * 
 * Two input file arguments are supplied as well as an output file argument.
 * 
 * 
 * The cross correlation between the magnitude waveforms of the two files is
 * stored in the output file
 * 
 * To find the autocorrelation, use the same file for both inputs to this program.
 * 
 * Typical usage is: ./magnitudeXCORR <file_1> <file_2> <output_file> <sampling rate (Hz)>
 * Example: 	file_1: motion_data_output_circle_pattern.csv
 * 		file_2: motion_data_output_circle_pattern.csv xcorr_out.csv
 * 		output file: xcorr_output.csv
 * 		sampling rate: 	20
 * 
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


void 
xcorr(float vector1[], float vector2[], float corr_1_2[], int n_max)
{
	int		delay;
	int		delay_max;
	int		i         , j;
	float		dot_1_2;
	float		m1 = 0;
	float		m2 = 0;

	delay_max = n_max;

	/*
	 * Compute dot products over delay values
	 */

	for (delay = -delay_max; delay <delay_max; delay++) {

		dot_1_2 = 0;

		/*
 		 * Set dot products to zero for delay
		 * exceeding vector length 
		 */ 

		for (i = 0; i < n_max; i++) {
			j = i + delay;
			if (j >= n_max || j < 0){
				continue;
			}
				dot_1_2 = dot_1_2 + (vector1[i] - m1) * (vector2[j] - m2);
		}
		corr_1_2[delay + delay_max] = dot_1_2;
	}
}

int 
main(int argc, char **argv)
{
	FILE           *fp;
	char           *ifile1_name, *ifile2_name, *ofile_name;
	char           *line = NULL;
	size_t		len = 0;
	ssize_t		read;
	int		N_SAMPLES;

	int		vector_length, vector_length1, vector_length2, rv, i;

	float          *time_vector1;
	float          *time_vector2;
	float	       *ampMagVec1;
	float          *ampMagVec2;
	float		sample_rate;
	float	       *crossCorrMags;
	
	if (argc != 5) {
		fprintf(stderr,
			"Error - check usage\n"
			);
		exit(EXIT_FAILURE);
	}
	ifile1_name = argv[1];
	ifile2_name = argv[2];
	ofile_name = argv[3];
	sample_rate = atof(argv[4]);

	/* open the input file1 */

	//printf(" Input file \'%s\'.\n", ifile1_name);
	fp = fopen(ifile1_name, "r");
	if (fp == NULL) {
		fprintf(stderr,
			"Failed to read from file \'%s\'.\n",
			ifile1_name
			);
		exit(EXIT_FAILURE);
	}
	/* count the number of lines in the file */

	read = getline(&line, &len, fp);
	//discard header of file
		N_SAMPLES = 0;
	while ((read = getline(&line, &len, fp)) != -1) {
		N_SAMPLES++;
	}

	//printf("N_SAMPLES %i\n", N_SAMPLES);

	/* go back to the start of the file so that the data can be read */
	rewind(fp);
	read = getline(&line, &len, fp);
	//discard header of file

	/* start reading the data from the file into the data structures */

	time_vector1 = (float *)malloc(sizeof(float) * N_SAMPLES);

	ampMagVec1 = (float *)malloc(sizeof(float) * N_SAMPLES);

	/*
	 * Note, remove lines that do not contain required values
	 */
	i = 0;
	while ((read = getline(&line, &len, fp)) != -1) {
		/* parse the data */
		rv = sscanf(line, "%f, %f\n", &time_vector1[i], &ampMagVec1[i]);
		if (rv != 2) {
			fprintf(stderr,
				"%s %d \'%s\'\n",
				"Skipping line",
				i,
				line
				);
			continue;
		}
		i++;
	}

	fclose(fp);

	vector_length1 = N_SAMPLES;

	/* open the input file2 */
	//printf(" Input file \'%s\'.\n", ifile2_name);
	fp = fopen(ifile2_name, "r");
	if (fp == NULL) {
		fprintf(stderr,
			"Failed to read from file \'%s\'.\n",
			ifile2_name
			);
		exit(EXIT_FAILURE);
	}
	/* count the number of lines in the file */
	read = getline(&line, &len, fp);
	//discard header of file
		N_SAMPLES = 0;
	while ((read = getline(&line, &len, fp)) != -1) {
		N_SAMPLES++;
	}

	vector_length2 = N_SAMPLES;

	/* go back to the start of the file so that the data can be read */
	rewind(fp);
	read = getline(&line, &len, fp);
	//discard header of file

	/* start reading the data from the file into the data structures */
	time_vector2 = (float *)malloc(sizeof(float) * N_SAMPLES);

	ampMagVec2 = (float *)malloc(sizeof(float) * N_SAMPLES);
	/*
	 * Note, remove lines that do not contain both peak and trough values
	 */
	i = 0;
	while ((read = getline(&line, &len, fp)) != -1) {
		/* parse the data */
		rv = sscanf(line, "%f, %f\n", &time_vector2[i], &ampMagVec2[i]);
		if (rv != 2) {
			fprintf(stderr,
				"%s %d \'%s\'\n",
				"Skipping line",
				i,
				line
				);
			continue;
		}
		i++;
	}
	fclose(fp);


	vector_length = vector_length2;
	if (vector_length1 < vector_length2) {
		vector_length = vector_length1;
	} else {
		vector_length = vector_length2;
	}

	i = 0;

	crossCorrMags = (float *)malloc(sizeof(float) * 2 * vector_length);

	//printf("Number of input samples: %i \n", vector_length);

	xcorr(ampMagVec1, ampMagVec2, crossCorrMags, vector_length);

	//printf("Cross correlation output file:  \'%s\'.\n", ofile_name);
	fp = fopen(ofile_name, "w");
	if (fp == NULL) {
		fprintf(stderr,
			"Failed to write to file \'%s\'.\n",
			ofile_name
			);
		exit(EXIT_FAILURE);
	}
	fprintf(fp, "Time offset	 XCorr of Magnitudes \n");
	for (i = 0; i < 2 * vector_length; i++) {
		fprintf(fp, "%f, %f\n",
			(float)(i - vector_length)/sample_rate,
			crossCorrMags[i]
			);
	}
	fclose(fp);

	return 0;
}
