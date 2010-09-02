# - Find neko
# Find the native MySQL includes and library
#
#  NEKO_INCLUDE_DIR - where to find mysql.h, etc.
#  NEKO_LIBRARIES   - List of libraries when using Neko.
#  NEKO_FOUND       - True if Neko found.

IF (NEKO_INCLUDE_DIR)
  # Already in cache, be silent
  SET(NEKO_FIND_QUIETLY TRUE)
ENDIF (NEKO_INCLUDE_DIR)

FIND_PATH(NEKO_INCLUDE_DIR 
  NAMES	neko.h
  PATHS /usr/local/include 
		/usr/include 
)

# Finally the library itself
find_library(NEKO_LIBRARY
  NAMES neko
  PATHS /usr/lib
		/usr/local/lib
)

SET(NEKO_NAMES neko)

IF (NEKO_INCLUDE_DIR AND NEKO_LIBRARY)
  SET(NEKO_FOUND TRUE)
  SET( NEKO_LIBRARIES ${NEKO_LIBRARY} )
ELSE (NEKO_INCLUDE_DIR AND NEKO_LIBRARY)
  SET(NEKO_FOUND FALSE)
  SET( NEKO_LIBRARIES )
ENDIF (NEKO_INCLUDE_DIR AND NEKO_LIBRARY)

# Just display a message about success or fail.
IF (NEKO_FOUND)
#  IF (NOT NEKO_FIND_QUIETLY)
    MESSAGE(STATUS "Found Neko: ${NEKO_LIBRARY}")
#  ENDIF (NOT NEKO_FIND_QUIETLY)
ELSE (NEKO_FOUND)
  IF (NEKO_FIND_REQUIRED)
    MESSAGE(STATUS "Looked for Neko libraries named ${NEKO_NAMES}.")
    MESSAGE(FATAL_ERROR "Could NOT find neko library")
  ENDIF (NEKO_FIND_REQUIRED)
ENDIF (NEKO_FOUND)

MARK_AS_ADVANCED(
  NEKO_LIBRARY
  NEKO_INCLUDE_DIR
 )
