# - Find Haxe 
#
#  HAXE_PROGRAM     - Haxe executable: haxe .
#  HAXE_FOUND       - True if Haxe found.


FIND_PROGRAM(HAXE_PROGRAM 
  NAMES	haxe
  PATHS /usr/local/bin 
		/usr/bin 
)

SET(HAXE_NAMES haxe)

IF (HAXE_PROGRAM)
  SET(HAXE_FOUND TRUE)
ELSE (HAXE_PROGRAM)
  SET(HAXE_FOUND FALSE)
ENDIF (HAXE_PROGRAM)

# Just display a message about success or fail.
IF (HAXE_FOUND)
    MESSAGE(STATUS "Found HAXE executable: ${HAXE_PROGRAM}")
ELSE (HAXE_FOUND)
  IF (HAXE_FIND_REQUIRED)
    MESSAGE(STATUS "Looked for HAXE executable named ${HAXE_NAMES}.")
    MESSAGE(FATAL_ERROR "Could NOT find haxe executable")
  ENDIF (HAXE_FIND_REQUIRED)
ENDIF (HAXE_FOUND)

MARK_AS_ADVANCED(
  HAXE_PROGRAM
 )
