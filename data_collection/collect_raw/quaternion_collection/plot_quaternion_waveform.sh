#
#Typical usage: ./plot_single_waveform.sh <file_name>
#Note: Depending on the file you choose, the gnuplot script may need to be changed
#

if [ "$#" -ne 1 ]; then
		echo "Please enter the name of the file you wish to plot"
		exit
	fi

cp $1 quaternion_test.csv
gnuplot quaternion_gnuplot_script
sudo cp test.png /var/www/html/graphics
rm quaternion_test.csv
