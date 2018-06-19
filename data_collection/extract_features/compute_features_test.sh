echo "Beginning Feature Computation"

if [ "$#" -ne 2 ]; then
		echo "Please enter a starting sample number and an ending sample number"
		exit
	fi

rm test_set.csv

for i in $(seq $1 $2);
do

	./waveform_peak_find ../raw/mdo_${i}w.csv ../features/x_peaks_${i}w.csv ../features/x_single-axis_${i}w.csv ../features/y_peaks_${i}w.csv ../features/y_single-axis_${i}w.csv ../features/z_peaks_${i}w.csv ../features/z_single-axis_${i}w.csv 100 100 100
	./waveform_peak_find ../raw/mdo_${i}e.csv ../features/x_peaks_${i}e.csv ../features/x_single-axis_${i}e.csv ../features/y_peaks_${i}e.csv ../features/y_single-axis_${i}e.csv ../features/z_peaks_${i}e.csv ../features/z_single-axis_${i}e.csv 100 100 100

	./truncateData ../raw/mdo_${i}w.csv ../features/z_peaks_${i}w.csv ../raw/truncatedData/trunc_mdo_${i}w.csv
	./truncateData ../raw/mdo_${i}e.csv ../features/z_peaks_${i}e.csv ../raw/truncatedData/trunc_mdo_${i}e.csv

	./accGyrMag ../raw/truncatedData/trunc_mdo_${i}w.csv ../raw/accMag_${i}w.csv ../raw/gyrMag_${i}w.csv
	./accGyrMag ../raw/truncatedData/trunc_mdo_${i}e.csv ../raw/accMag_${i}e.csv ../raw/gyrMag_${i}e.csv

	./findAbsExtrema ../raw/truncatedData/trunc_mdo_${i}w.csv ../features/accMaxMin_${i}w.csv ../features/gyrMaxMin_${i}w.csv
	./findAbsExtrema ../raw/truncatedData/trunc_mdo_${i}e.csv ../features/accMaxMin_${i}e.csv ../features/gyrMaxMin_${i}e.csv

	./findMagnitudeExtrema ../raw/accMag_${i}e.csv ../features/accTimeFeature_${i}e.csv
	./findMagnitudeExtrema ../raw/gyrMag_${i}e.csv ../features/gyrTimeFeature_${i}e.csv
	./findMagnitudeExtrema ../raw/accMag_${i}w.csv ../features/accTimeFeature_${i}w.csv
	./findMagnitudeExtrema ../raw/gyrMag_${i}w.csv ../features/gyrTimeFeature_${i}w.csv
	
	./magnitudeXCORR ../raw/accMag_${i}e.csv ../raw/accMag_${i}w.csv ../features/accXCORR_${i}.csv 20
	./magnitudeXCORR ../raw/gyrMag_${i}e.csv ../raw/gyrMag_${i}w.csv ../features/gyrXCORR_${i}.csv 20
	./magnitudeXCORR ../raw/accMag_${i}e.csv ../raw/gyrMag_${i}e.csv ../features/eXCORR_${i}.csv 20
	./magnitudeXCORR ../raw/accMag_${i}w.csv ../raw/gyrMag_${i}w.csv ../features/wXCORR_${i}.csv 20

	./computeRiemannSum ../raw/truncatedData/trunc_mdo_${i}w.csv ../features/riemann_${i}w.csv
	./computeRiemannSum ../raw/truncatedData/trunc_mdo_${i}e.csv ../features/riemann_${i}e.csv

	./features_to_csv ../features/accMaxMin_${i}w.csv ../features/accMaxMin_${i}e.csv ../features/gyrMaxMin_${i}w.csv ../features/gyrMaxMin_${i}e.csv ../features/accTimeFeature_${i}w.csv ../features/accTimeFeature_${i}e.csv ../features/gyrTimeFeature_${i}w.csv ../features/gyrTimeFeature_${i}e.csv ../features/riemann_${i}w.csv ../features/riemann_${i}e.csv test_set.csv

#./compute_cross_correlation_peaks.sh ../raw/mdo_${i}e.csv
#cp waveform_peaks_output_x.csv ../raw/mdo_${i}_Px.csv
#cp waveform_peaks_output_y.csv ../raw/mdo_${i}_Py.csv
#cp xcorr_waveform_peaks_output_x.csv ../raw/mdo_${i}_xcorr_Pxy.csv
#cp xcorr_waveform_peaks_output_y.csv ../raw/mdo_${i}_xcorr_Pyx.csv
#cp xcorr_extrema.dat ../raw/mdo_${i}_xcorr_extrema.dat
#./bubble_sort_norm ../raw/mdo_${i}_xcorr_Pxy.csv ../raw/mdo_${i}_xcorr_Pyx.csv ../raw/mdo_${i}_xcorr_Pxy_sortnorm.csv ../raw/mdo_${i}_xcorr_Pyx_sortnorm.csv
#./bubble_sort_norm ../raw/mdo_${i}_Px.csv ../raw/mdo_${i}_Py.csv ../raw/mdo_${i}_Px_sortnorm.csv ../raw/mdo_${i}_Py_sortnorm.csv
done

echo "Feature Extraction Complete"
echo ""
