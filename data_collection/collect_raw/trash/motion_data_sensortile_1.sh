#!/bin/bash
#
# Enter Device MAC Address and output file name as required
#
# Note 10 second acquisition period
#

#echo -e 'disconnect C0:83:2B:31:5A:48' | bluetoothctl

gatttool -b C0:83:2B:31:5A:48 -t random --char-write-req --handle=0x0012 --value=0100 --listen > sensor_data_stream_1.dat  &

sleep 5 

pkill gatttool
