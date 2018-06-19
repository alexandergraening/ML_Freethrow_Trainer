all: Acquire_Continuous_Wrist Acquire_Continuous_Elbow sample_file_sync Acquire_LowPass_Continuous_1 Acquire_LowPass_Continuous_2

Acquire_Continuous_Wrist: Acquire_Continuous_Wrist.c
	 	gcc -o Acquire_Continuous_Wrist Acquire_Continuous_Wrist.c -lm -lc -lrt	

Acquire_Continuous_Elbow: Acquire_Continuous_Elbow.c
		gcc -o Acquire_Continuous_Elbow Acquire_Continuous_Elbow.c -lm -lc -lrt

sample_file_sync: sample_file_sync.c
		gcc -o sample_file_sync sample_file_sync.c -lm

Acquire_LowPass_Continuous_1: Acquire_LowPass_Continuous_1.c
		gcc -o Acquire_LowPass_Continuous_1 Acquire_LowPass_Continuous_1.c -lm -lc -lrt -lliquid

Acquire_LowPass_Continuous_2: Acquire_LowPass_Continuous_2.c
		gcc -o Acquire_LowPass_Continuous_2 Acquire_LowPass_Continuous_2.c -lm -lc -lrt -lliquid

clean:
		rm -rf Acquire_Continuous_Wrist
		rm -rf Acquire_Continuous_Elbow
		rm -rf sample_file_sync
