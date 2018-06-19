echo ""
echo "Beginning XCORR computation of Elbow Acc with Wrist Acc"
echo ""

for i in $(seq 1 60);
do
	./magnitudeXCORR ../raw/accMag_${i}e.csv ../raw/accMag_${i}w.csv ../features/accXCORR_${i}.csv 20
	./magnitudeXCORR ../raw/gyrMag_${i}e.csv ../raw/gyrMag_${i}w.csv ../features/gyrXCORR_${i}.csv 20
	./magnitudeXCORR ../raw/accMag_${i}w.csv ../raw/gyrMag_${i}w.csv ../features/wXCORR_${i}.csv 20
	./magnitudeXCORR ../raw/accMag_${i}e.csv ../raw/gyrMag_${i}e.csv ../features/eXCORR_${i}.csv 20
done


