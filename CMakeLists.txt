#
# (C) 2010 NICTA

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(OI_MAJOR 0)
SET(OI_MINOR 1)
SET(OI_PATCH 10)

MESSAGE(STATUS "Configuring OiVibartions ${ARMA_MAJOR}.${ARMA_MINOR}.${ARMA_PATCH}")

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "./cmake/Modules/")

#INCLUDE(CheckIncludeFileCXX)
#INCLUDE(CheckLibraryExists)
#INCLUDE(FindBoost)


PROJECT(OiVibrations CXX)

SET(ARMA_USE_NEKO false)
SET(ARMA_USE_MYSQL false)
SET(ARMA_USE_BOOST  false)

MESSAGE(STATUS "*** ")
MESSAGE(STATUS "*** The OiVibrations run-time library component will be an alias for the following libraries:")
MESSAGE(STATUS "*** OI_LIBS = ${OI_LIBS}")
MESSAGE(STATUS "*** ")

FIND_PACKAGE(Boost)
FIND_PACKAGE(Neko)
FIND_PACKAGE(Mysql++)

IF(MYSQLPP_FOUND)
    INCLUDE_DIRECTORIES(${MYSQLPP_INCLUDE_DIR})   
    MESSAGE(STATUS "Mysql++_INCLUDE_DIR = ${MYSQLPP_INCLUDE_DIR}")
    INCLUDE_DIRECTORIES(${MYSQL_INCLUDE_DIR})
    MESSAGE(STATUS "Mysql_INCLUDE_DIR = ${MYSQL_INCLUDE_DIR}")
#    LINK_DIRECTORIES(${MYSQLPP_LIBRARIES})
ENDIF(MYSQLPP_FOUND)


IF(Boost_FOUND)
  
  MESSAGE(STATUS "Boost_MAJOR_VERSION = ${Boost_MAJOR_VERSION}")
  MESSAGE(STATUS "Boost_MINOR_VERSION = ${Boost_MINOR_VERSION}")
  
  IF(Boost_MAJOR_VERSION GREATER 0)
    IF(Boost_MINOR_VERSION GREATER 33)
      SET(ARMA_USE_BOOST true)
      MESSAGE(STATUS "Boost_INCLUDE_DIR = ${Boost_INCLUDE_DIR}")
    ENDIF(Boost_MINOR_VERSION GREATER 33)
  ENDIF(Boost_MAJOR_VERSION GREATER 0)
  
ENDIF(Boost_FOUND)

IF(ARMA_USE_BOOST STREQUAL false)
  MESSAGE(STATUS "Boost libraries either not found or their version is too low (version 1.34 or later is good).")
  MESSAGE(STATUS "( It's possible that CMake didn't find the particular version of Boost you may have. )")
ENDIF(ARMA_USE_BOOST STREQUAL false)


# If Boost libraries were found, explicitly check if Boost's date_time library is also present.
# This is due to the non-standard packaging of Boost libraries on Debian and Ubuntu systems,
# where certain individual Boost libraries are packaged separately.

#IF(ARMA_USE_BOOST STREQUAL true)
  #SET(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} ${Boost_INCLUDE_DIR})
  #CHECK_INCLUDE_FILE_CXX("boost/date_time/posix_time/posix_time.hpp" ARMA_USE_BOOST_DATE)
#ENDIF(ARMA_USE_BOOST STREQUAL true)


IF(ARMA_USE_BOOST STREQUAL true)
  INCLUDE_DIRECTORIES(include ${Boost_INCLUDE_DIR})
ELSE(ARMA_USE_BOOST STREQUAL true)
  INCLUDE_DIRECTORIES(include)
ENDIF(ARMA_USE_BOOST STREQUAL true)


## For any library that is not in a default location,
## embed its path into the Armadillo runtime library.
## Examples of default locations are "/lib", "/usr/lib",
## or as specified in "/etc/ld.so.conf".
##
## Path embedding is not recommended unless you know
## what you're doing.  It might be better to add the
## path to the "/etc/ld.so.conf" file and then run "ldconfig".

FILE(GLOB folder_source *.cpp)
FILE(GLOB folder_header *.h)
SOURCE_GROUP("Source files" FILES ${folder_source})
SOURCE_GROUP("Header files" FILES ${folder_header})

ADD_LIBRARY( OiVibrations SHARED ${folder_source} )
#TARGET_LINK_LIBRARIES( OiVibrations ${ARMA_LIBS} )
TARGET_LINK_LIBRARIES( OiVibrations mysqlpp )

#SET_TARGET_PROPERTIES(OiVibartions PROPERTIES VERSION ${ARMA_MAJOR}.${ARMA_MINOR}.${ARMA_PATCH} SOVERSION 0)

# As Red Hat Enterprise Linux (and related systems such as Fedora)
# does not search /usr/local/lib by default, we need to place the
# library in /usr

#IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  #SET(CMAKE_INSTALL_PREFIX "/usr")
#ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

#MESSAGE(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")


## Allow for the "lib" directory to be specified on the command line

#IF(NOT LIB_INSTALL_DIR)
  #IF(UNIX AND NOT APPLE)   # I don't know how Mac OS handles 64 bit systems
    #IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
      #MESSAGE(STATUS "Detected 64 bit system")
      #SET(LIB_INSTALL_DIR "lib64")
    #ELSE(CMAKE_SIZEOF_VOID_P EQUAL 8)
      #SET(LIB_INSTALL_DIR "lib")
    #ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 8)
  
  #ELSE(UNIX AND NOT APPLE)
    #SET(LIB_INSTALL_DIR "lib")
  #ENDIF(UNIX AND NOT APPLE)
#ENDIF(NOT LIB_INSTALL_DIR)

#MESSAGE(STATUS "LIB_INSTALL_DIR = ${LIB_INSTALL_DIR}")

## Allow for the "include" directory to be specified on the command line

#IF(NOT INCLUDE_INSTALL_DIR)
  #SET(INCLUDE_INSTALL_DIR "include")
#ENDIF(NOT INCLUDE_INSTALL_DIR)

#MESSAGE(STATUS "INCLUDE_INSTALL_DIR = ${INCLUDE_INSTALL_DIR}")


## Note that the trailing / character in "include/" is critical

#INSTALL(DIRECTORY include/ DESTINATION ${INCLUDE_INSTALL_DIR}
#PATTERN ".svn" EXCLUDE
#PATTERN "*.cmake" EXCLUDE
#PATTERN "*~" EXCLUDE
#PATTERN "*orig" EXCLUDE
#)

#INSTALL(TARGETS OiVibrations DESTINATION ${LIB_INSTALL_DIR})

