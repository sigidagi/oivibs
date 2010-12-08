cmake_minimum_required(VERSION 2.6)

#INCLUDE_DIRECTORIES("../")
#LINK_DIRECTORIES("../") 

SET(CMAKE_CXX_FLAGS "-g -Wall")

ADD_EXECUTABLE(nekotest test.cpp)
TARGET_LINK_LIBRARIES(nekotest dl neko )
