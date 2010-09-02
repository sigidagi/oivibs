# - Find Linear Algerba library Armadillo 
#
#  ARMA_INCLUDE_DIR - where to find iarmadillo file.
#  ARMA_LIBRARY   - List of libraries when using armadillo.
#  ARMA_FOUND       - True if armadillo found.

IF (ARMA_INCLUDE_DIR)
  # Already in cache, be silent
  SET(ARMA_FIND_QUIETLY TRUE)
ENDIF (ARMA_INCLUDE_DIR)

FIND_PATH(ARMA_INCLUDE_DIR 
  NAMES armadillo	
  PATHS /usr/local/include 
		/usr/include 
)

# Finally the library itself
FIND_LIBRARY(ARMA_LIBRARY
  NAME armadillo 
  PATHS /usr/lib
		/usr/local/lib
)

SET(NEKO_ARMA armadillo)

IF (ARMA_INCLUDE_DIR AND ARMA_LIBRARY)
  SET(ARMA_FOUND TRUE)
  SET( ARMA_LIBRARIES ${ARMA_LIBRARY} )
ELSE (ARMA_INCLUDE_DIR AND ARMA_LIBRARY)
  SET(ARMA_FOUND FALSE)
  SET( ARMA_LIBRARIES )
ENDIF (ARMA_INCLUDE_DIR AND ARMA_LIBRARY)

# Just display a message about success or fail.
IF (ARMA_FOUND)
    MESSAGE(STATUS "Found Armadillo: ${ARMA_LIBRARY}")
ELSE (ARMA_FOUND)
  IF (ARMA_FIND_REQUIRED)
    MESSAGE(STATUS "Looked for Armadillo libraries named ${ARMA_NAMES}.")
    MESSAGE(FATAL_ERROR "Could NOT find armadillo library")
  ENDIF (ARMA_FIND_REQUIRED)
ENDIF (ARMA_FOUND)

MARK_AS_ADVANCED(
  ARMA_LIBRARY
  ARMA_INCLUDE_DIR
 )
