cmake_minimum_required(VERSION 3.0.0)
project(ex1 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(ex1 MainTrain.cpp SimpleAnomalyDetector.cpp anomaly_detection_util.cpp timeseries.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
