// =====================================================================================
// 
//       Filename:  OiMusic.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-03-12 22:06:06
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

#ifndef  OIMUSIC_INC
#define  OIMUSIC_INC

#include	<armadillo>
#include	<OiFFTWest.h>

using namespace arma;

namespace Oi {

    class Music
    {
        private:
            FFTWest gfft_;
            int segmentLength_;
            double samplingInterval_;
            int overlap_;
            bool calcCSD_;
            
            cx_cube CSD_;
        public:
            Music();
            Music(int segmentLength);
            Music(int segmentLength, double sampling);
            Music(int segmentLength, double sampling, bool calcCSD);
    
            void setSamplingInterval(double sampling);
            void setSegmentLength(int length);
            void setCalculateCSD(bool calcCSD);

            void apply(const mat& in, mat& out);
            cx_cube& getCSD();
    };


} // namespace Oi


#endif   // ----- #ifndef OIMUSIC_INC  -----
