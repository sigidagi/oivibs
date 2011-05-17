PROJECT(OIVIBS)

MESSAGE(STATUS "Generating 'config.hpp'")
CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/build/config.hpp.cmake" "${OIVIBS_BINARY_DIR}/config.hpp")

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} ${OIVIBS_SOURCE_DIR} ${OIVIBS_BINARY_DIR})

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

#IF(OI_USE_MYSQLPP)
    #MESSAGE(STATUS "Mysql++ is found. Set variable OI_USE_MYSQLPP  = ${OI_USE_MYSQLPP}")
    
    #TARGET_LINK_LIBRARIES( OiVibrations mysqlpp armadillo)
    #IF(NEKO_FOUND)
        #ADD_LIBRARY( Vibrations SHARED ${folder_source} client_neko/NekoInterface.cpp )
        #SET_TARGET_PROPERTIES( Vibrations
            #PROPERTIES 
                #LIBRARY_OUTPUT_DIRECTORY ./client_neko
                #SUFFIX .ndll 
                #PREFIX Oi )
        #TARGET_LINK_LIBRARIES( Vibrations mysqlpp armadillo neko)
        #ADD_SUBDIRECTORY(client_neko)
    #ENDIF(NEKO_FOUND)
#ENDIF(OI_USE_MYSQLPP)

TARGET_LINK_LIBRARIES( OiVibrations ${OI_LIBS})
#ADD_SUBDIRECTORY(client)

ADD_EXECUTABLE(vibclient client/test.cpp)
TARGET_LINK_LIBRARIES(vibclient OiVibrations)


#ENABLE_TESTING()
#ADD_TEST(SimpleTest ./test/dlltest /var/www/uploads/test2.uff)

#SET_TARGET_PROPERTIES(OiVibartions PROPERTIES VERSION ${ARMA_MAJOR}.${ARMA_MINOR}.${ARMA_PATCH} SOVERSION 0)

# As Red Hat Enterprise Linux (and related systems such as Fedora)
# does not search /usr/local/lib by default, we need to place the
# library in /usr

#IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  #SET(CMAKE_INSTALL_PREFIX "/usr")
#ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

#MESSAGE(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")

# Allow for the "lib" directory to be specified on the command line
IF(NOT LIB_INSTALL_DIR)
  IF(UNIX AND NOT APPLE)   # I don't know how Mac OS handles 64 bit systems
    IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
      MESSAGE(STATUS "Detected 64 bit system")
      SET(LIB_INSTALL_DIR "lib64")
    ELSE(CMAKE_SIZEOF_VOID_P EQUAL 8)
      SET(LIB_INSTALL_DIR "lib")
    ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 8)
  
  ELSE(UNIX AND NOT APPLE)
    SET(LIB_INSTALL_DIR "lib")
  ENDIF(UNIX AND NOT APPLE)
ENDIF(NOT LIB_INSTALL_DIR)

MESSAGE(STATUS "LIB_INSTALL_DIR = ${LIB_INSTALL_DIR}")

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

INSTALL(TARGETS OiVibrations vibclient 
        LIBRARY DESTINATION /usr/${LIB_INSTALL_DIR}
        RUNTIME DESTINATION /usr/bin
)

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Sigitas Dagilis") 
 
INCLUDE(CPack)
# To use this:
# make package
# sudo dpkg -i OiVibrations-0.1.1-Linux.deb
