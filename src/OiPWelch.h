// =====================================================================================
// 
//       Filename:  OiPWelch.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-03-12 21:50:52
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Sigitas Dagilis (), sigidagi@gmail.com
//
//     This file is part of the OiVibrations C++ library.
//     It is provided without any warranty of fitness
//     for any purpose. You can redistribute this file
//     and/or modify it under the terms of the GNU
//     General Public License (GPL) as published
//     by the Free Software Foundation, either version 3
//     of the License or (at your option) any later version.
//     (see http://www.opensource.org/licenses for more info)
// 
// =====================================================================================

#ifndef  OIPWELCH_INC
#define  OIPWELCH_INC

#include	<armadillo>
#include	<OiFFTWest.h>

using namespace arma;

namespace Oi {

    class PWelch
    {
        private:
            FFTWest gfft_;
            int segmentLength_;
            int overlap_;
            double samplingInterval_;
            bool calcCSD_;

            cx_cube CSD_;
        public:
            PWelch();
            PWelch(int segmentLength);
            PWelch(int segmentLength, int overlap);
            PWelch(int segmentLength, int overlap, double sampling);
            PWelch(int segmentLength, int overlap, double sampling, bool calcCSD_);
           
            void setSegmentLength(int length);
            void setSamplingInterval(double sampling);
            void setOverlap(int overlap);
            void setCalculateCSD(bool calcCSD);

            void apply(const mat& in, mat& out); 
            cx_cube& getCSD();
    };
} // namespace Oi

#endif   // ----- #ifndef OIPWELCH_INC  -----
