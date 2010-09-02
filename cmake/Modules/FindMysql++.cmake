# - Try to find ImageMagick++
# Once done, this will define
#
#  MYSQLPP_FOUND - system has Magick++
#  MYSQLPP_INCLUDE_DIRS - the Magick++ include directories
#  MYSQLPP_LIBRARIES - link these to use Magick++

include(LibFindMacros)

# Dependencies
libfind_package(Mysql++ Mysql)

# Include dir
find_path(MYSQLPP_INCLUDE_DIR 
	NAMES mysql++.h 
	PATHS /usr/include/mysql++ 
		  /usr/local/include/mysql++
)

# Finally the library itself
find_library(MYSQLPP_LIBRARY
  NAMES mysqlpp
  PATHS /usr/lib
		/usr/local/lib
)

SET(MYSQLPP_NAMES mysqlpp)

IF (MYSQLPP_INCLUDE_DIR AND MYSQLPP_LIBRARY)
  SET(MYSQLPP_FOUND TRUE)
  SET( MYSQLPP_LIBRARIES ${MYSQLPP_LIBRARY} )
ELSE (MYSQLPP_INCLUDE_DIR AND MYSQLPP_LIBRARY)
  SET(MYSQLPP_FOUND FALSE)
  SET( MYSQLPP_LIBRARIES )
ENDIF (MYSQLPP_INCLUDE_DIR AND MYSQLPP_LIBRARY)


IF (MYSQLPP_FOUND)
  IF (NOT MYSQLPP_FIND_QUIETLY)
    MESSAGE(STATUS "Found MySQL++: ${MYSQLPP_LIBRARY}")
  ENDIF (NOT MYSQLPP_FIND_QUIETLY)
ELSE (MYSQLPP_FOUND)
  IF (MYSQLPP_FIND_REQUIRED)
    MESSAGE(STATUS "Looked for MySQL++ libraries named ${MYSQLPP_NAMES}.")
    MESSAGE(FATAL_ERROR "Could NOT find MySQL++ library")
  ENDIF (MYSQLPP_FIND_REQUIRED)
ENDIF (MYSQLPP_FOUND)

MARK_AS_ADVANCED(
  MYSQLPP_LIBRARY
  MYSQLPP_INCLUDE_DIR
  )

