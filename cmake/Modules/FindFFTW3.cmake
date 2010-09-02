# Copyright (c) 2009, Whispersoft s.r.l.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
# * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
# * Neither the name of Whispersoft s.r.l. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Finds FFTW3 library
#
#  FFTW3_INCLUDE_DIR - where to find fftw3.h, etc.
#  FFTW3_LIBRARIES   - List of libraries when using FFTW3.
#  FFTW3_FOUND       - True if FFTW3 found.
#

if (FFTW3_INCLUDE_DIR)
  # Already in cache, be silent
  set(FFTW3_FIND_QUIETLY TRUE)
endif (FFTW3_INCLUDE_DIR)

find_path(FFTW3_INCLUDE_DIR fftw3.h
  /opt/local/include
  /usr/local/include
  /usr/include
)

set(FFTW3_NAMES fftw3)
find_library(FFTW3_LIBRARY
  NAMES ${FFTW3_NAMES}
  PATHS /usr/lib /usr/local/lib /opt/local/lib
)

if (FFTW3_INCLUDE_DIR AND FFTW3_LIBRARY)
   set(FFTW3_FOUND TRUE)
   set( FFTW3_LIBRARIES ${FFTW3_LIBRARY} )
else (FFTW3_INCLUDE_DIR AND FFTW3_LIBRARY)
   set(FFTW3_FOUND FALSE)
   set(FFTW3_LIBRARIES)
endif (FFTW3_INCLUDE_DIR AND FFTW3_LIBRARY)

if (FFTW3_FOUND)
   if (NOT FFTW3_FIND_QUIETLY)
      message(STATUS "Found FFTW3: ${FFTW3_LIBRARY}")
   endif (NOT FFTW3_FIND_QUIETLY)
else (FFTW3_FOUND)
   if (FFTW3_FIND_REQUIRED)
      message(STATUS "Looked for FFTW3 libraries named ${FFTW3_NAMES}.")
      message(STATUS "Include file detected: [${FFTW3_INCLUDE_DIR}].")
      message(STATUS "Lib file detected: [${FFTW3_LIBRARY}].")
      message(FATAL_ERROR "=========> Could NOT find FFTW3 library")
   endif (FFTW3_FIND_REQUIRED)
endif (FFTW3_FOUND)

mark_as_advanced(
  FFTW3_LIBRARY
  FFTW3_INCLUDE_DIR
  )
