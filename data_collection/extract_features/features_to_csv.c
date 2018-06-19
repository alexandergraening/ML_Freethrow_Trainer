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
 *	./features_to_csv <accMaxMin_w> <accMaxMin_e> <gyrMaxMin_w> <gyrMaxMin_e> <accTimeFeature_w> <accTimeFeature_e> <gyrTimeFeature_w> <gyrTimeFeature_e> <riemann_w> <riemann_e> <OUTPUT_FILE>
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
	FILE *ofp,*ifp;
	char *ofile_name;
        char *ifile_name;
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        int N_SAMPLES;
	int vector_length, rv, i;
	//float xMA,yMA,zMA,xMG,yMG,zMG,xMAt,xmAt,yMAt,ymAt,zMAt,zmAt,xMGt,xmGt,yMGt,ymGt,zMGt,zmGt;	//Raw
	float xAy,yAz,zAx,xGy,yGz,zGx,dMmxA,dMmyA,dMmzA,dMmxG,dMmyG,dMmzG;	//Computed Features
	float acc_raw_vector[12];
	float gyr_raw_vector[12];
	float aMag,gMag;
	//char* file_strt_accMm = "../features/accMaxMin_";
	//char* file_strt_gyrMm = "../features/gyrMaxMin_";
	//char* file_strt_accTf = "../features/accTimeFeature_";
	//char* file_strt_gyrTf = "../features/gyrTimeFeature_";
	//char* file_ext_w = "w.csv\0";
	//char* file_ext_e = "e.csv\0";

	float * time_vector;
	float * amplitude_vector;
	float * time_vector2;
	float * amplitude_vector2;

	if (argc != 12) {
		printf("Error - check usage (<input_file> <start_sample> <end_sample>)\n");
		exit(EXIT_FAILURE);
	}

	//InputFile names are as follows:
	//	../raw/truncatedData/trunc_mdo1w.csv
	//	../raw/truncatedData/trunc_mdo1e.csv
	//	../features/accMaxMin_1w.csv
	//	../features/accMaxMin_1e.csv
	//	../features/gyrMaxMin_1w.csv
	//	../features/gyrMaxMin_1e.csv
	//	../features/accTimeFeature_1w.csv
	//	../features/accTimeFeature_1e.csv
	//	../features/gyrTimeFeature_1w.csv
	//	../features/gyrTimeFeature_1e.csv

	ofile_name = argv[11];
	ofp = fopen(ofile_name, "a");
	if (ofp == NULL)
	{
		printf("Failed to open output file. \'%s\' Check path.\n",ofile_name);
		exit(EXIT_FAILURE);
	}

	//Begin iterations:
	int k;
	int max_iterator_val = 1;//*argv[3]-0x30;
	for (k = 0; k < max_iterator_val; ++k)
	{
		//printf("k = %d\tmax_iterator_val = %d\n",k,max_iterator_val);
		//
		//First do steps for the wrist data
		//
		//
		/*ifile_name[0] = '\0';
		strcat(ifile_name,file_strt_accMm);
		ifile_name[strlen(ifile_name)]=k+0x30;
		ifile_name[strlen(ifile_name)+1]='\0';
		strcat(ifile_name,file_ext_w);*/

		ifile_name = argv[1];

		ifp = fopen(ifile_name,"r");
		if (ifp == NULL)
		{
			printf("Failed to open \'%s\'\n",ifile_name);	
			dMmxA = 0;dMmyA = 0;dMmzA = 0;
			xAy = 0;yAz = 0;zAx = 0;
		}
		else
		{
		
		//Do stuff here.
        	read = getline(&line, &len, ifp); //discard header of file
        	read = getline(&line, &len, ifp); //Load file data into line.                

		rv = sscanf(line,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",&acc_raw_vector[0],&acc_raw_vector[1],&acc_raw_vector[2],&acc_raw_vector[3],&acc_raw_vector[4],&acc_raw_vector[5],&acc_raw_vector[6],&acc_raw_vector[7],&acc_raw_vector[8],&acc_raw_vector[9],&acc_raw_vector[10],&acc_raw_vector[11]);
		aMag = sqrt(acc_raw_vector[3]*acc_raw_vector[3]+acc_raw_vector[7]*acc_raw_vector[7]+acc_raw_vector[11]*acc_raw_vector[11]);

        	read = getline(&line, &len, ifp); //Load file data into line.
        	read = getline(&line, &len, ifp); //Load file data into line.

                rv = sscanf(line, "%f,%f,%f\n", &dMmxA,&dMmyA,&dMmzA);

		if (rv != 3)
		{
			printf("Failure to extract time feature data from axis file, printing dummy values!\n");
			dMmxA = 0;dMmyA = 0;dMmzA = 0;
		}

        	read = getline(&line, &len, ifp); //Discard Header.                
        	read = getline(&line, &len, ifp); //Load file data into line.

		rv = sscanf(line, "%f,%f,%f\n",&xAy,&yAz,&zAx);
		if (rv != 3)
		{
			printf("Failure to extract ratio data from axis file, printing dummy values!\n");
			xAy = 0;yAz = 0;zAx = 0;
		}
		}
		fclose(ifp);


/*		ifile_name[0] = '\0';
		strcat(ifile_name,file_strt_gyrMm);
		ifile_name[strlen(ifile_name)]=k+0x30;
		ifile_name[strlen(ifile_name)+1]='\0';
		strcat(ifile_name,file_ext_w);*/

		ifile_name = argv[3];

		ifp = fopen(ifile_name,"r");
		if (ifp == NULL)
		{
			printf("Failed to open %s\'\n",ifile_name);	
			dMmxG = 0;dMmyG = 0;dMmzG = 0;
			xGy = 0;yGz = 0;zGx = 0;
		}
		else
		{

		//Do stuff here.
        	read = getline(&line, &len, ifp); //discard header of file
        	read = getline(&line, &len, ifp); //Load file data into line.                
		rv = sscanf(line,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",&gyr_raw_vector[0],&gyr_raw_vector[1],&gyr_raw_vector[2],&gyr_raw_vector[3],&gyr_raw_vector[4],&gyr_raw_vector[5],&gyr_raw_vector[6],&gyr_raw_vector[7],&gyr_raw_vector[8],&gyr_raw_vector[9],&gyr_raw_vector[10],&gyr_raw_vector[11]);
		gMag = sqrt(gyr_raw_vector[3]*gyr_raw_vector[3]+gyr_raw_vector[7]*gyr_raw_vector[7]+gyr_raw_vector[11]*gyr_raw_vector[11]);
        	read = getline(&line, &len, ifp); //Load file data into line.
        	read = getline(&line, &len, ifp); //Load file data into line.
		
                rv = sscanf(line, "%f,%f,%f\n", &dMmxG,&dMmyG,&dMmzG);
		if (rv != 3)
		{
			printf("Failure to extract time feature data from axis file, printing dummy values!\n");
			dMmxG = 0;dMmyG = 0;dMmzG = 0;
		}

        	read = getline(&line, &len, ifp); //Discard Header.                
        	read = getline(&line, &len, ifp); //Load file data into line.
		
		rv = sscanf(line, "%f,%f,%f,\n",&xGy,&yGz,&zGx);
		if (rv != 3)
		{
			printf("Failure to extract ratio data from axis file, printing dummy values!\n");
			xGy = 0;yGz = 0;zGx = 0;
		}
		}
		fclose(ifp);

		//dMmxA = 0;dMmyA = 0;dMmzA = 0;dMmxG = 0;dMmyG = 0;dMmzG = 0;
		/*ifile_name = "";
		strcat(ifile_name,file_strt_accTf);
		ifile_name[strlen(ifile_name)]=k+0x30;
		ifile_name[strlen(ifile_name)+1]='\0';
		strcat(ifile_name,file_ext_w);

		ifp = fopen(ifile_name,"r");
		if (ifp == NULL)
		{
			fprintf(stderr,"Failed to open \'%s\'\n",ofile_name);
			exit(EXIT_FAILURE);
		}

		//Do stuff here.

		fclose(ifp);


		ifile_name = "";
		strcat(ifile_name,file_strt_gyrTf);
		ifile_name[strlen(ifile_name)]=k+0x30;
		ifile_name[strlen(ifile_name)+1]='\0';
		strcat(ifile_name,file_ext_w);

		ifp = fopen(ifile_name,"r");
		if (ifp == NULL)
		{
			fprintf(stderr,"Failed to open \'%s\'\n",ofile_name);
			exit(EXIT_FAILURE);
		}

		//Do stuff here.

		fclose(ifp);

		//Write to output file here
*/
		//fprintf(ofp,"%d,w,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n\r",k,xAy,yAz,zAx,xGy,yGz,zGx,dMmxA,dMmyA,dMmzA,dMmxG,dMmyG,dMmzG);
		
		//fprintf(ofp,"%f\t%f\t",aMag,gMag);
		//Normalized Values for ML training
		double aMagNorm,gMagNorm;
		aMagNorm = (aMag-3000)/3000;
		gMagNorm = (gMag-11500)/11500;
		fprintf(ofp,"%f\t%f\t",aMagNorm,gMagNorm);

		//
		//Then do stuff for Elbow data
		//
		//

/*		ifile_name[0] = '\0';
		strcat(ifile_name,file_strt_accMm);
		ifile_name[strlen(ifile_name)]=k+0x30;
		ifile_name[strlen(ifile_name)+1]='\0';
		strcat(ifile_name,file_ext_e);*/

		ifile_name = argv[2];

		ifp = fopen(ifile_name,"r");
		if (ifp == NULL)
		{
			printf("Failed to open \'%s\'\n",ifile_name);
			xAy = 0;yAz = 0;zAx = 0;		
			dMmxA = 0;dMmyA = 0;dMmzA = 0;
		}
		else
		{

		//Do stuff here.
        	read = getline(&line, &len, ifp); //discard header of file
        	read = getline(&line, &len, ifp); //Load file data into line.                
		rv = sscanf(line,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",&acc_raw_vector[0],&acc_raw_vector[1],&acc_raw_vector[2],&acc_raw_vector[3],&acc_raw_vector[4],&acc_raw_vector[5],&acc_raw_vector[6],&acc_raw_vector[7],&acc_raw_vector[8],&acc_raw_vector[9],&acc_raw_vector[10],&acc_raw_vector[11]);
		aMag = sqrt(acc_raw_vector[3]*acc_raw_vector[3]+acc_raw_vector[7]*acc_raw_vector[7]+acc_raw_vector[11]*acc_raw_vector[11]);
        	read = getline(&line, &len, ifp); //Load file data into line.
        	read = getline(&line, &len, ifp); //Load file data into line.
		
                rv = sscanf(line, "%f,%f,%f\n", &dMmxA,&dMmyA,&dMmzA);
		if (rv != 3)
		{
			printf("Failure to extract time feature data from axis file, printing dummy values!\n");
			dMmxA = 0;dMmyA = 0;dMmzA = 0;
		}

        	read = getline(&line, &len, ifp); //Discard Header.                
        	read = getline(&line, &len, ifp); //Load file data into line.
		
		rv = sscanf(line, "%f,%f,%f\n",&xAy,&yAz,&zAx);
		if (rv != 3)
		{
			printf("Failure to extract ratio data from axis file, printing dummy values!\n");
			xAy = 0;yAz = 0;zAx = 0;
		}
		}
		fclose(ifp);


/*		ifile_name[0] = '\0';
		strcat(ifile_name,file_strt_gyrMm);
		ifile_name[strlen(ifile_name)]=k+0x30;
		ifile_name[strlen(ifile_name)+1]='\0';
		strcat(ifile_name,file_ext_e);*/
		ifile_name = argv[4];

		ifp = fopen(ifile_name,"r");
		if (ifp == NULL)
		{
			printf("Failed to open %s\'\n",ifile_name);	
			dMmxG = 0;dMmyG = 0;dMmzG = 0;
			xGy = 0;yGz = 0;zGx = 0;
		}
		else
		{
		//Do stuff here.
        	read = getline(&line, &len, ifp); //discard header of file
        	read = getline(&line, &len, ifp); //Load file data into line.                
		rv = sscanf(line,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",&gyr_raw_vector[0],&gyr_raw_vector[1],&gyr_raw_vector[2],&gyr_raw_vector[3],&gyr_raw_vector[4],&gyr_raw_vector[5],&gyr_raw_vector[6],&gyr_raw_vector[7],&gyr_raw_vector[8],&gyr_raw_vector[9],&gyr_raw_vector[10],&gyr_raw_vector[11]);
		gMag = sqrt(gyr_raw_vector[3]*gyr_raw_vector[3]+gyr_raw_vector[7]*gyr_raw_vector[7]+gyr_raw_vector[11]*gyr_raw_vector[11]);
        	read = getline(&line, &len, ifp); //Load file data into line.
        	read = getline(&line, &len, ifp); //Load file data into line.
		
                rv = sscanf(line, "%f,%f,%f\n",&dMmxG,&dMmyG,&dMmzG);
		if (rv != 3)
		{
			printf("Failure to extract time feature data from axis file, printing dummy values!\n");
			dMmxG = 0;dMmyG = 0;dMmzG = 0;
		}

        	read = getline(&line, &len, ifp); //Discard Header.                
        	read = getline(&line, &len, ifp); //Load file data into line.
		
		rv = sscanf(line, "%f,%f,%f",&xGy,&yGz,&zGx);
		if (rv != 3)
		{
			printf("Failure to extract ratio data from axis file, printing dummy values!\n");
			xGy = 0;yGz = 0;zGx = 0;
		}
		}
		fclose(ifp);
		
		aMagNorm = (aMag-3000)/3000;
		gMagNorm = (gMag-11500)/11500;
		fprintf(ofp,"%f\t%f\t",aMagNorm,gMagNorm);
		//fprintf(ofp,"%f\t%f\t",aMag,gMag);

		ifile_name = argv[9];
		ifp = fopen(ifile_name,"r");
		double magnitude0;
		double magnitude1;
		double magnitude2;
		double magnitude3;
		double magnitude4;
		double magnitude5;
		if (ifp == NULL)
		{
			printf("Failed to open %s\'\n",ifile_name);	
			magnitude0 = 0;
			magnitude1 = 0;
			magnitude2 = 0;
			magnitude3 = 0;
			magnitude4 = 0;
			magnitude5 = 0;
		}
		else
		{
        		read = getline(&line, &len, ifp); //Load file data into line.
			rv = sscanf(line, "%f,%f,%f,%f,%f,%f",&magnitude0,&magnitude1,&magnitude2,&magnitude3,&magnitude4,&magnitude5);
			int k;
			for (k = 0; k < strlen(line);++k)
			{
				if (line[k] == '\n')
					line[k] = '\0';
			}
			fprintf(ofp,"%s",line);
		}

		fclose(ifp);

		ifile_name = argv[10];
		ifp = fopen(ifile_name,"r");

		if (ifp == NULL)
		{
			printf("Failed to open %s\'\n",ifile_name);	
			magnitude0 = 0;
			magnitude1 = 0;
			magnitude2 = 0;
			magnitude3 = 0;
			magnitude4 = 0;
			magnitude5 = 0;
		}
		else
		{
        		read = getline(&line, &len, ifp); //Load file data into line.
			rv = sscanf(line, "%f,%f,%f,%f,%f,%f",&magnitude0,&magnitude1,&magnitude2,&magnitude3,&magnitude4,&magnitude5);
			fprintf(ofp,"%s",line);
		}

		fclose(ifp);
		//dMmxA = 0;dMmyA = 0;dMmzA = 0;dMmxG = 0;dMmyG = 0;dMmzG = 0;
	/*	ifile_name = "";
		strcat(ifile_name,file_strt_accTf);
		ifile_name[strlen(ifile_name)]=k+0x30;
		ifile_name[strlen(ifile_name)+1]='\0';
		strcat(ifile_name,file_ext_e);

		ifp = fopen(ifile_name,"r");
		if (ifp == NULL)
		{
			fprintf(stderr,"Failed to open \'%s\'\n",ofile_name);
			exit(EXIT_FAILURE);
		}
		
		//Do stuff here.

		fclose(ifp);


		ifile_name = "";
		strcat(ifile_name,file_strt_gyrTf);
		ifile_name[strlen(ifile_name)]=k+0x30;
		ifile_name[strlen(ifile_name)+1]='\0';
		strcat(ifile_name,file_ext_e);

		ifp = fopen(ifile_name,"r");
		if (ifp == NULL)
		{
			fprintf(stderr,"Failed to open \'%s\'\n",ofile_name);
			exit(EXIT_FAILURE);
		}

		//Do stuff here.

		fclose(ifp);


		//Print to output file here:
*/
		//fprintf(ofp,"%d,e,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n\r",k,xAy,yAz,zAx,xGy,yGz,zGx,dMmxA,dMmyA,dMmzA,dMmxG,dMmyG,dMmzG);


	}

	fclose(ofp);
	return 0;


/*




        read = getline(&line, &len, fp); //discard header of file
        N_SAMPLES = 0;
        while ((read = getline(&line, &len, fp)) != -1) {
                N_SAMPLES++;
        }


        rewind(fp);
        read = getline(&line, &len, fp); //discard header of file

	N_SAMPLES--;



        i = 0;
        time_vector = (float *) malloc(sizeof(float) * N_SAMPLES);
        amplitude_vector  = (float *) malloc(sizeof(float) * N_SAMPLES);
        while ((read = getline(&line, &len, fp)) != -1) {

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


        printf(" Input file \'%s\'.\n", ifile_name2);
        fp = fopen(ifile_name2, "r");
        if (fp == NULL) {
                fprintf(stderr, 
                                "Failed to read from file \'%s\'.\n", 
                                ifile_name2
                       );
                exit(EXIT_FAILURE);
        }


        read = getline(&line, &len, fp); //discard header of file
        N_SAMPLES2 = 0;
        while ((read = getline(&line, &len, fp)) != -1) {
                N_SAMPLES2++;
        }


        rewind(fp);
        read = getline(&line, &len, fp); //discard header of file



        i = 0;
        time_vector2 = (float *) malloc(sizeof(float) * N_SAMPLES2);
        amplitude_vector2  = (float *) malloc(sizeof(float) * N_SAMPLES2);
        while ((read = getline(&line, &len, fp)) != -1) {

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

	// Normalize the time vectors
	norm = time_vector3[vector_length-1];
	norm_vector(time_vector3, &norm, vector_length);

	norm_vector(time_vector, &norm, N_SAMPLES);
	norm_vector(time_vector2, &norm, N_SAMPLES2);

	// Normalize the amp vectors
	min_vector(amplitude_vector3, &vec_min, vector_length);
	max_vector(amplitude_vector3, &vec_max, vector_length);
	printf("min %f max %f\n", vec_min, vec_max);
	norm = vec_max - vec_min;
	norm_vector(amplitude_vector3, &norm, vector_length);
  	norm_vector(amplitude_vector, &norm, N_SAMPLES);
	norm_vector(amplitude_vector2, &norm, N_SAMPLES2);     

        
	printf("Sorted and normalized data file \'%s\'.\n", ofile_name);
        fp = fopen(ofile_name, "w");
        if (fp == NULL) {
                fprintf(stderr, 
                                "Failed to write to file \'%s\'.\n", 
                                ofile_name
                       );
                exit(EXIT_FAILURE);
        }



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

    	return 0;*/
}

