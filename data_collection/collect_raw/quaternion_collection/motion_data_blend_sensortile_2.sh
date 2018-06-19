#bin/bash

gatttool -b C0:83:49:30:4B:4D -t random --char-write-req --handle=0x0012 --value=0100; gatttool -b C0:83:49:30:4B:4D -t random --char-write-req --handle=0x001e --value=0100 --listen > sensor_data_stream_2.dat &

sleep 5

pkill gatttool
