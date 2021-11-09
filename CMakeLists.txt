cmake_minimum_required(VERSION 3.20)
project(anomaly_detection_util)

set(CMAKE_CXX_STANDARD 11)

add_executable(anomaly_detection_util MainTrain.cpp anomaly_detection_util.cpp timeseries.cpp SimpleAnomalyDetector.cpp)