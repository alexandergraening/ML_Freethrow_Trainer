//
//  Based on Autofilter.c 
//  Created by Charles Zaloom on 1/28/18.
//
//
//  Execute:
//  ./Acquire_Continuous_Wrist <POSIX_TIME>
//  Argument 1 for POSIX time stamp for scheduled execution of data acquisition 
//
//  Compile:
//
//  gcc -o Acquire_LowPass_Continuous_1 Acquire_LowPass_Continuous_1.c -lm -lc -lliquid -lrt
//
//
//  Relies on gatttool interface to BLE Motion system on SensorTile developed by Xu Zhang
//
//
//  Requires motion_data.sh shell script
//
//  Requires that proper device MAC Address be entered into gatttool command in 
//  motion_data.sh shell script
//
//  Produces output data file: motion_data_output.csv
//


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <liquid/liquid.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>

const int BUFF_MAX = 256;

#define OUTPUT_FILE "motion_data_output_q1.csv"

#define SAMPLE_PERIOD 0.050

const char SIGNAL_QI[] = "tempfile_qi_1.txt";
const char SIGNAL_QJ[] = "tempfile_qj_1.txt";
const char SIGNAL_QK[] = "tempfile_qk_1.txt";

int char_to_decimal(char letter){

	switch(letter){
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		case 'a':
			return 10;
		case 'b':
			return 11;
		case 'c':
			return 12;
		case 'd':
			return 13;
		case 'e':
			return 14;
		case 'f':
			return 15;
		default:
			return -1;
	}
}

int hex_to_decimal_4bit(char seq[4]){//, int twos_comp){
	int sum = char_to_decimal(seq[1]);
	sum += (char_to_decimal(seq[0])*16);
	sum += (char_to_decimal(seq[3])*16*16);
	sum += (char_to_decimal(seq[2])*16*16*16);

	if(sum > 32767) sum = sum - 65536; //twos_comp == 1 && 

	return sum;
}

int hex_to_decimal_time(char seq[4]){
	int sum2 = char_to_decimal(seq[1]);
	sum2 += (char_to_decimal(seq[0])*16);
	int sum1 = char_to_decimal(seq[3]);
	sum1 += (char_to_decimal(seq[2])*16); 
	sum1 = (sum1 * 1000) + sum2;
	return sum1;
}

int stream_parser(char raw[BUFF_MAX]){
	int i = 0;
	while(i < BUFF_MAX && raw[i] != '='){ i++; }
	i += 7;
	if (raw[i] == '1')
	{
		return 2;		
	}
	while(i < BUFF_MAX && raw[i] != ':'){ i++; }
	i += 2;
	char *ptr = &raw[i];

	int iter = 0;
	char data[40]; 
	int lock = 0;
	while(iter < 40 && ptr != NULL && *ptr != '\0' && *ptr != '\n'){
		if(lock < 40 && *ptr != ' '){ 
			data[iter] = *ptr;
			++lock;
			++iter;
		}
       	ptr++;
    }

    FILE * out_qi = fopen(SIGNAL_QI,"a");
    FILE * out_qj = fopen(SIGNAL_QJ,"a");
    FILE * out_qk = fopen(SIGNAL_QK,"a");
    
    int y;
	int index;
   	index = 0; 
    for(y = 0; y < 10; y++){
		switch(y){
			case 0:
				//sensortile.timestamp = hex_to_decimal_time(&data[y]);
				break;
			case 1:
				fprintf(out_qi, "%f\n", (float)(hex_to_decimal_4bit(&data[index]))); 
				break;
			case 2:
				fprintf(out_qj, "%f\n", (float)(hex_to_decimal_4bit(&data[index]))); 
				break;
			case 3:
				fprintf(out_qk, "%f\n", (float)(hex_to_decimal_4bit(&data[index]))); 
				break;
			case 4:
				fprintf(out_qi, "%f\n", (float)(hex_to_decimal_4bit(&data[index])));
				break;
			case 5:
				fprintf(out_qj, "%f\n", (float)(hex_to_decimal_4bit(&data[index])));
				break;
			case 6: 
				fprintf(out_qk, "%f\n", (float)(hex_to_decimal_4bit(&data[index])));
				break;
			case 7:
				fprintf(out_qi, "%f\n", (float)(hex_to_decimal_4bit(&data[index])));
				break;
			case 8:
				fprintf(out_qj, "%f\n", (float)(hex_to_decimal_4bit(&data[index])));
				break;
			case 9: 
				fprintf(out_qk, "%f\n", (float)(hex_to_decimal_4bit(&data[index])));
				break;
			default:
				return 0;
    	}
	index = index + 4;
    }

    fclose(out_qi);
    fclose(out_qj);
    fclose(out_qk);

    return 1;
}      

unsigned int BLE_parse(const char *inFile){
	FILE* ble_file;
	ble_file = fopen(inFile, "r");

	char raw[BUFF_MAX];

	//
	// Advance line over first line of file 
	// since first line of file may be concatenated 
	// or may contain header
	//

	fgets(raw, BUFF_MAX, ble_file);

	// Read motion data

	unsigned int iter = 0;
	while(fgets(raw, BUFF_MAX, ble_file)){
		int result = stream_parser(raw);
		if(result == 0) return 0;
		if(result == 1)	iter++;
	}
	//
	// Decrement iter to ensure that last line of file is eliminated
	// since last line may be concatenated in data transfer
	//
	
	return iter--;
}

void makeCSV(unsigned int size){
	FILE *output = fopen(OUTPUT_FILE, "w");
	fprintf(output, "timestamp,qi,qj,qk\n");

	FILE * out_qi = fopen(SIGNAL_QI,"r");
    	FILE * out_qj = fopen(SIGNAL_QJ,"r");
    	FILE * out_qk = fopen(SIGNAL_QK,"r");
	
    	unsigned int i;
    	float qi,qj,qk;
	float timestamp = 0;
    	for(i=0; i<size; ++i){
		
		timestamp = i*SAMPLE_PERIOD;

		fscanf(out_qi,"%f",&qi);
		fscanf(out_qj,"%f",&qj);
		fscanf(out_qk,"%f",&qk);

    		fprintf(output, "%f,%f,%f,%f\n",timestamp,qi,qj,qk);
    	}
    		fclose(output);

    		fclose(out_qi);
    		fclose(out_qj);
    		fclose(out_qk);
	}

void rms_comp(const char *signal, unsigned int n_samples, float * t_start, float * t_stop, float * rms_signal) {

	float x[n_samples];
	float num;
	unsigned int i;
	float sample_time;
	float sample_period;
	float signal_mean;
	int sample_count;

	sample_period = SAMPLE_PERIOD;

	FILE *fp = fopen(signal,"r");

        if(fp == 0){
                printf("INVALID FILE (filter)\n");
                return;
        }

	sample_time = 0;
	signal_mean = 0;
	sample_count = 0;
	//
	// Compute mean signal
	//
        for (i=0; i<n_samples; i++){
                sample_time = sample_time + sample_period;
		if (sample_time > * t_start && sample_time < * t_stop){
			fscanf(fp,"%f",&num);
                	x[sample_count] = num;
			signal_mean = signal_mean + x[sample_count];
			sample_count++;
		}
	}

	sample_count--;

	if(sample_count > 0){
		signal_mean = signal_mean / sample_count;
	}

	//
	// Compute rms of zero mean signal within time window
	// 

	*rms_signal = 0;
        for (i=0; i<sample_count; i++){
		x[i] = x[i] - signal_mean;
		*rms_signal = *rms_signal + x[i]*x[i];
        }

	*rms_signal = (float)(*rms_signal)/sample_count;
	*rms_signal = (float)sqrt(*rms_signal);
        fclose(fp);
}


void cleanup(){

	if (remove(SIGNAL_QI) != 0)
      		printf("Unable to delete tempfile_qi.txt");
 	if (remove(SIGNAL_QJ) != 0)
    		printf("Unable to delete tempfile_qj.txt");
    	if (remove(SIGNAL_QK) != 0)
    		printf("Unable to delete tempfile_qk.txt");

}


void sig_handler_data_acq_1(int handler_val)
{
			struct timespec handler_exec_time;
			clock_gettime(CLOCK_REALTIME, &handler_exec_time);
			printf("Current POSIX time at start of data acquisition 1 seconds %i\n",handler_exec_time.tv_sec);
			printf("Current POSIX time at start of data acquisition 1 nanoseconds %i\n",handler_exec_time.tv_nsec);
			system("sh motion_data_blend_sensortile_1.sh");
}

int main(int argc, char *argv[]) {	

	float t_start, t_stop;
	float rms_signal;
	unsigned int size;
	char * input_file;
	int n_cycles;
	int cycle_count;
	int filter_enable;
	struct timespec current_time;	
	struct itimerspec timer_input_1, timer_output_1;
	int timer_posix_value;
	int return_value_1;
	struct sigevent sig_1;
	timer_t data_qua_timer_1;

	if(argc != 2 ){
		printf("Please provide POSIX start time\n");
		return 0;
		}
		
	timer_posix_value  = atoi(argv[1]);

	sig_1.sigev_notify = SIGEV_SIGNAL;
	sig_1.sigev_signo = SIGUSR1;
	signal(SIGUSR1, sig_handler_data_acq_1);

	return_value_1 = timer_create(CLOCK_REALTIME, &sig_1, &data_qua_timer_1);
	if (return_value_1 != 0){
			printf("Timer create error:  %d\n", errno);
			timer_delete(data_qua_timer_1);
			return 0;
	}


	printf("POSIX data quaternion time %i\n",timer_posix_value);

	timer_input_1.it_value.tv_sec = timer_posix_value;
	timer_input_1.it_value.tv_nsec = 0;
	timer_input_1.it_interval.tv_sec = 0;
	timer_input_1.it_interval.tv_nsec = 0;

					
/*
 * 	Start timer
 */

	return_value_1 = timer_settime(data_qua_timer_1, 1, &timer_input_1, &timer_output_1);

	if (return_value_1 == 0)
		sleep(15);
		else {
		printf("Timer create error:  %d\n", errno);
		timer_delete(data_qua_timer_1);
		return 0;
	}

	system("tail -n 199 sensor_data_stream_1.dat > motion_data_1.dat");

	input_file = "motion_data_1.dat";

	size = 3*BLE_parse(input_file);
	if(size == 0){
			printf("ERROR (stream_parser): BLE Data formatted incorrectly.(Wrist)\n");
		    	return 0;
    	}

	printf(" Number of samples acquired =  %i \n", size);

	t_start = SAMPLE_PERIOD;;
	t_stop  = size*SAMPLE_PERIOD;
	
	rms_comp(SIGNAL_QI,size,&t_start, &t_stop, &rms_signal);

	printf(" RMS signal amplitude over time window t_start %f to t_step %f = %f\n", t_start, t_stop, rms_signal);

	makeCSV(size);

	printf(" Filtered motion data for cycle written to output data file\n");

	cleanup();


    	return 0;
}
