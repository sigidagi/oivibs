cmake_minimum_required(VERSION 2.6)

#INCLUDE_DIRECTORIES("../")
#LINK_DIRECTORIES("../") 

SET(CMAKE_CXX_FLAGS "-g -Wall -pthread")

ADD_EXECUTABLE(dlltest test.cpp)
TARGET_LINK_LIBRARIES(dlltest dl)
