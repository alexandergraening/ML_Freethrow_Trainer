UCLA Winter and Spring Quarters 2018
Class: EE180D
Professor: William J. Kaiser
Authors: Alexander Graening and Jim Xu

Goal: Create a program to recognize and provide feedback on freethrow form that adapts to individual users. This will provide a step forward in adaptive automated coaching in sports and sports medicine. Our ideal system would be used by someone practicing freethrows as follows:
1. User would wear sensor/sensors strapped to the back of the hand and/or close to the elbow.
2. Upon preparing to take a freethrow, user would execute a simple gesture sequence that can easily be recognized. (Likely simply reliant on orientation with respect to gravity.)
3. User then takes a freethrow and observes the result. Based on the result (make, miss right, miss left, miss short, miss long), the user will provide a gesture indicating the actual result.
4. The system will compare the actual result to a predicted result based on collected data. If the two agree, the system will either say "good job" or provide a suggestion as to where your primary issue was. (Pay attention to your elbow, pay attention to your hand, pay attention to your jump, etc.)

We will build the system in a series of incremental steps:
1. Build a freethrow classifier:
	- Design a simple data collection program
	- Video and collect data for 60 freethrows
	- Hand-label the data
	- Observe waveforms and select features
	- Divide the collected data in half and train using the first half based on the selected features and observed results, then use the classifier to predict the results of the other half and compare the results with the observed results. (Do not move on from this step until 90%+ accuracy is achieved.)
2. Test using additional data from the same person.
	- Build test program and test this in the field.
	- Add secondary classifier that will isolate different features to determine the reason for a miss. (elbow, wrist, jump, etc.)
3. Polish and finalize system
	- If results are good, add interface for gesture labelling and automated feedback to increase size of training set
	- Add pruning algorithm to ensure the data set stays a manageable size and throws away non-ideal data samples.


Work log:
Thursday Week 1:
	Synchronize two SensorTiles with data collection program
	Create data collection program and script to collect multiple sets
Notes: Look into complementary filter (as what AP uses) to measure starting/ending angles.

Collect initial data:
	60 full motions

	x number of motions for each step
		- Shooting from above the head
		- Raising the ball

Thursday Week 2:
	In the past week, we encountered issues with synchronizing two SensorTiles. It appears that as soon as gatttool is used ot connect to one sensor tile and it initializes bluetooth, both SensorTiles are connected. This results in an error when the second gatttool command is called, since one of the sensors is already connected.
	Selected 0.2 as the ideal argument for filtering input data with AcquireSensorData
	Modified max sensor readings for the accelerometer.
	Features we want to explore:
		- Ratio between y and z gyroscope peak magnitudes (This will likely determine the aim.)
		- Acceleration vector magnitude (This will likely determine distance) (Maybe maximum, maybe average, maybe integral?)
		- Cross Correlations? (We will look into this if we need more features. It is almost certain we will need additional features.)
		- Research Complementary filter to track orientaion? (This might allow us to get away from being reliant on exactly how the sensor is strapped to the hand.)

Week 3: 
	This past week, we collected a new set of freethrow data for 60 freethrows using a slightly better corner frequency to filter our data. We also wrote a significant portion of the code we will be using to extract features from our data. Here are some of the advancements we made:
		- automated computation of vector magnitude files that we can graph to show the change in the absolute magnitude of the acceleration and the rotation for each of our motions
		- computation of absolute minimums and maximums for each axis long with their time of occurrence and writing of this data to a separate file for each motion
		- Modification of the cross correlation system to allow us to better select which sets of data to use as input.
		We also did some maintenance to make our system easier to use:
		- Learned how to write Makefiles so we could start using these to more easily compile our programs
		- Studied the gnuplot function and adapted this to enable us to plot our data more easily
		- Added functionality that makes it easy for us to take a large set of data (for example 60 samples) and note where or if our trails were contaminated or produced errors, then easily rerun the data for very specific trials (such as trial 5, 13, and 17) without having to rerun the entire set. This will be especially useful since it will allow us to reuse this code for later in-the-field testing.



Week 4:
	Integrated cross correlation code with the rest of the project
	Added a "plot_single_waveform.sh" script to plot a single waveform at a time to allow easier inspection of certain file waveforms
	Fixed bug that would prevent easy plotting
	Cleaned up code to minimize the number of programs that run to extract features and removed print statements that run when code is working. (Only left error message statements)
	We spent most of today looking for features.
	Alex worked on plotting and examining waveforms
	Jim worked on different quantitative ratios and compared them in a spreadsheet
	Nothing definitive yet.

