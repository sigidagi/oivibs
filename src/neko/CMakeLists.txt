cmake_minimum_required(VERSION 2.6)

#INCLUDE_DIRECTORIES("../")
#LINK_DIRECTORIES("../") 

SET(CMAKE_CXX_FLAGS "-g -Wall")

ADD_EXECUTABLE(oitest test.cpp)
TARGET_LINK_LIBRARIES(oitest dl neko )
