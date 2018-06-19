all: waveform_peak_find accGyrMag waveform_feature_xcorr magnitudeXCORR findAbsExtrema findMagnitudeExtrema truncateData computeRiemannSum features_to_csv

waveform_peak_find: waveform_peak_find.c
	gcc -o waveform_peak_find waveform_peak_find.c -lm

accGyrMag: accGyrMag.c
	gcc -o accGyrMag accGyrMag.c -lm

waveform_feature_xcorr: waveform_feature_xcorr.c
	gcc -o waveform_feature_xcorr waveform_feature_xcorr.c -lm

magnitudeXCORR: magnitudeXCORR.c
	gcc -o magnitudeXCORR magnitudeXCORR.c -lm

findAbsExtrema: findAbsExtrema.c
	gcc -o findAbsExtrema findAbsExtrema.c

findMagniudeExtrema: findMagnitudeExtrema.c
	gcc -o findMagnitudeExtrema findMagnitudeExtrema.c

truncateData: truncateData.c
	gcc -o truncateData truncateData.c

computeRiemannSum: computeRiemannSum.c
	gcc -o computeRiemannSum computeRiemannSum.c

features_to_csv: features_to_csv.c
	gcc -o features_to_csv features_to_csv.c -lm

clean:
	find . -type f | xargs touch
	rm -rf waveform_peak_find *~
	rm -rf accGyrMag
	rm -rf waveform_feature_xcorr
	rm -rf magnitudeXCORR
	rm -rf findAbsExtrema
	rm -rf findMagnitudeExtrema
	rm -rf computeRiemannSum
