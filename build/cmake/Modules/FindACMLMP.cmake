# - Find AMD's ACMLMP library (no includes) which provides optimised and parallelised BLAS and LAPACK functions
# This module defines
#  ACMLMP_LIBRARIES, the libraries needed to use ACMLMP.
#  ACMLMP_FOUND, If false, do not try to use ACMLMP.
# also defined, but not for general use are
#  ACMLMP_LIBRARY, where to find the ACMLMP library.

SET(ACMLMP_NAMES ${ACMLMP_NAMES} acml_mp)
FIND_LIBRARY(ACMLMP_LIBRARY
  NAMES ${ACMLMP_NAMES}
  PATHS /usr/lib64 /usr/lib /usr/*/lib64 /usr/*/lib /usr/*/gfortran64_mp/lib/ /usr/*/gfortran32_mp/lib/ /usr/local/lib64 /usr/local/lib /opt/lib64 /opt/lib /opt/*/lib64 /opt/*/lib /opt/*/gfortran64_mp/lib/ /opt/*/gfortran32_mp/lib/
  )

# Include dir
FIND_PATH(ACMLMP_INCLUDE_DIR 
	NAMES acml.h 
    PATHS /usr/include /usr/*/include  /usr/*/gfortran64_mp/include/ /usr/*/gfortran32_mp/include/ /usr/local/include
    /opt/include /opt/*/include /opt/*/gfortran64_mp/include/ /opt/*/gfortran32_mp/include/
  )

# handle the QUIETLY and REQUIRED arguments and set ACMLMP_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Armadillo DEFAULT_MSG ACMLMP_LIBRARY ACMLMP_INCLUDE_DIR)

MARK_AS_ADVANCED(
  ACMLMP_INCLUDE_DIR
  ACMLMP_LIBRARY
  )
