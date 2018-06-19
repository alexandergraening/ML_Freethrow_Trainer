#!/bin/bash
#
# Usage sensor_sample.sh -t <TIME IN SECONDS> 
#
#

#echo 'remove C0:83:2B:31:5A:48' | bluetoothctl
#echo 'remove C0:83:49:30:4B:4D' | bluetoothctl

while getopts t: option
do
	case "${option}"
	in
 		   t) TIME_OFFSET=${OPTARG};;
        esac
done

CURRENT_TIME=$(date +%s)

echo Current Time: $CURRENT_TIME
TRIGGER_TIME=$(($CURRENT_TIME+$TIME_OFFSET))
echo Trigger Time: $TRIGGER_TIME


./Acquire_LowPass_Continuous_2 0.2 $TRIGGER_TIME 1  &
./Acquire_LowPass_Continuous_1 0.2 $TRIGGER_TIME 1  &
