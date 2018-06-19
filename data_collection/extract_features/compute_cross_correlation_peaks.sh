#!/bin/sh

#
# Usagei: enter input file name and peak find threshold values for X and Y axes 
#


if [ "$#" -ne 1 ]; then
	    echo "Enter Input File Name"
	    exit
    fi

sh compute_cross_correlation.sh $1 $1 20

./xcorr_waveform_peak_find xcorr_output.csv xcorr_waveform_peaks_output_x.csv xcorr_waveform_gnuplot_x.csv xcorr_waveform_peaks_output_y.csv xcorr_waveform_gnuplot_y.csv 100000 100000
#
# Uncomment the below for plotting capability
#
#gnuplot xcorr_cross_corr_waveform_gnuplot_script
#sudo cp xcorr_cross_corr_waveform_plot.png /var/www/html/graphics 
#echo "If no error is reported, view the plot image at http://<Beaglebone IP Address>:8080/graphics/waveform_plot.png"

sh find_waveform_peaks.sh $1 80 80

