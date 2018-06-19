#bin/bash

gatttool -b C0:83:2B:31:5A:48 -t random --char-write-req --handle=0x0012 --value=0100; gatttool -b C0:83:2B:31:5A:48 -t random --char-write-req --handle=0x001e --value=0100 --listen > sensor_data_stream_1.dat &

