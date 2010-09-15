
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

SET(CMAKE_CXX_FLAGS "-g -Wall")

SET(OI_USE_NEKO false)
SET(OI_USE_MYSQLPP false)
SET(OI_USE_BOOST  false)

MESSAGE(STATUS "*** ")
MESSAGE(STATUS "*** The OiVibrations run-time library component will be an alias for the following libraries:")
MESSAGE(STATUS "*** OI_LIBS = ${OI_LIBS}")
MESSAGE(STATUS "*** ")

FIND_PACKAGE(Boost QUIET)
FIND_PACKAGE(Neko)
FIND_PACKAGE(Mysql++)
FIND_PACKAGE(Armadillo REQUIRED)

IF(MYSQLPP_FOUND)
    INCLUDE_DIRECTORIES(${MYSQLPP_INCLUDE_DIR})   
    MESSAGE(STATUS "Mysql++_INCLUDE_DIR = ${MYSQLPP_INCLUDE_DIR}")
    INCLUDE_DIRECTORIES(${MYSQL_INCLUDE_DIR})
    MESSAGE(STATUS "Mysql_INCLUDE_DIR = ${MYSQL_INCLUDE_DIR}")
    SET(OI_USE_MYSQLPP true)
ENDIF(MYSQLPP_FOUND)


IF(Boost_FOUND)
  MESSAGE(STATUS "Boost_MAJOR_VERSION = ${Boost_MAJOR_VERSION}")
  MESSAGE(STATUS "Boost_MINOR_VERSION = ${Boost_MINOR_VERSION}")
  
  IF(Boost_MAJOR_VERSION GREATER 0)
    IF(Boost_MINOR_VERSION GREATER 33)
      SET(OI_USE_BOOST true)
      MESSAGE(STATUS "Boost_INCLUDE_DIR = ${Boost_INCLUDE_DIR}")
    ENDIF(Boost_MINOR_VERSION GREATER 33)
  ENDIF(Boost_MAJOR_VERSION GREATER 0)
 
  SET(OI_USE_BOOST true)
ENDIF(Boost_FOUND)

IF(OI_USE_BOOST STREQUAL false)
  MESSAGE(STATUS "Boost libraries either not found or their version is too low (version 1.34 or later is good).")
  MESSAGE(STATUS "( It's possible that CMake didn't find the particular version of Boost you may have. )")
ENDIF(OI_USE_BOOST STREQUAL false)


IF(NEKO_FOUND)
    SET(OI_USE_NEKO true)
ENDIF(NEKO_FOUND)

IF(OI_USE_BOOST STREQUAL true)
  INCLUDE_DIRECTORIES(include ${Boost_INCLUDE_DIR})
ELSE(OI_USE_BOOST STREQUAL true)
  INCLUDE_DIRECTORIES(include)
ENDIF(OI_USE_BOOST STREQUAL true)

MESSAGE(STATUS "Generating 'config.hpp'")
CONFIGURE_FILE(config.hpp.cmake config.hpp)


## For any library that is not in a default location,
## embed its path into the OiVibrations runtime library.
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
TARGET_LINK_LIBRARIES( OiVibrations mysqlpp armadillo )

IF(NEKO_FOUND)
    ADD_LIBRARY( Vibrations SHARED ${folder_source} neko/NekoInterface.cpp )
    SET_TARGET_PROPERTIES( Vibrations
        PROPERTIES 
            LIBRARY_OUTPUT_DIRECTORY ./neko
            SUFFIX .ndll 
            PREFIX Oi )
     
    TARGET_LINK_LIBRARIES( Vibrations mysqlpp armadillo )
    
    ADD_SUBDIRECTORY(test)

ENDIF(NEKO_FOUND)


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

