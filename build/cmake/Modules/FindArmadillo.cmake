#  Find Linear Algerba library Armadillo 
#
#  ARMADILLO_INCLUDE_DIR - where to find iarmadillo file.
#  ARMADILLO_LIBRARY   - List of libraries when using armadillo.
#  ARMADILLO_FOUND       - True if armadillo found.

IF (ARMADILLO_INCLUDE_DIR)
  # Already in cache, be silent
   SET(ARMADILLO_FIND_QUIETLY TRUE)
ENDIF (ARMADILLO_INCLUDE_DIR)

FIND_PATH(ARMADILLO_INCLUDE_DIR 
  NAMES armadillo	
  PATHS /usr/local/include 
		/usr/include 
)

# Finally the library itself
FIND_LIBRARY(ARMADILLO_LIBRARY
  NAME armadillo 
  PATHS /usr/lib
		/usr/local/lib
)

# handle the QUIETLY and REQUIRED arguments and set ARMADILLO_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Armadillo DEFAULT_MSG ARMADILLO_LIBRARY ARMADILLO_INCLUDE_DIR)

MARK_AS_ADVANCED(
  ARMADILLO_LIBRARY
  ARMADILLO_INCLUDE_DIR
 )
