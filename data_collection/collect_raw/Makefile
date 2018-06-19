all: Acquire_LowPass_Continuous_1 Acquire_LowPass_Continuous_2 wait_for_verification sample_file_sync writeTrainData

Acquire_LowPass_Continuous_1: Acquire_LowPass_Continuous_1.c
	 	gcc -o Acquire_LowPass_Continuous_1 Acquire_LowPass_Continuous_1.c -lm -lc -lliquid -lrt	

Acquire_LowPass_Continuous_2: Acquire_LowPass_Continuous_2.c
		gcc -o Acquire_LowPass_Continuous_2 Acquire_LowPass_Continuous_2.c -lm -lc -lliquid -lrt

wait_for_verification: wait_for_verification.c
		gcc -o wait_for_verification wait_for_verification.c

sample_file_sync: sample_file_sync.c
		gcc -o sample_file_sync sample_file_sync.c -lm

writeTrainData: writeTrainData.c
		gcc -o writeTrainData writeTrainData.c

clean:
		rm -rf Acquire_LowPass_Continuous_1
		rm -rf Acquire_LowPass_Continuous_2
		rm -rf sample_file_sync
		rm -rf wait_for_verification
		rm -rf writeTrainData
