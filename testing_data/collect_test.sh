cd ../data_collection/collect_raw
./Collect_Initial_Test_And_Train_Data.sh 0 0
cd ../extract_features
./compute_features_test.sh 0 0

cd ../../testing_data
cp ../data_collection/extract_features/test_set.csv test_features
