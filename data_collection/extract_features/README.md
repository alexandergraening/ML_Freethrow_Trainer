This folder contains the feature extraction files used for the Freethrow project.
The structure will be as follows:
	compute_features.sh will be the primary file to run feature extractrion on the data collection code
	compute_features_test.sh will be the primary file to run individual tests on one set of data.




List of modified files in this directory (feel free to blindly modify everything else, but be methodical with these:
	compute_features.sh
	compute_features_test.sh
	



TODO:

Compute waveforms for magnitude of acceleration for elbow and wrist.

Important cross correlation computations:
	Vector magnitude of AccelerationElbow crossed with AccelerationWrist
	Vector magnitude of GyroElbow crossed with GyroWrist
	Vector magnitude of AccelerationWrist crossed with GyroWrist
	Vector magnitude of AccelerationElbow crossed with GyroElbow

Compute Max and Min for x,y,z for acceleration and gyro.
Compute time values for each of these.
