// =====================================================================================
// 
//       Filename:  OiMath.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-03-02 12:02:46
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

#ifndef  OIMATH_INC
#define  OIMATH_INC

#include	"OiFFTWest.h"
#include	<armadillo>
#include	<complex>

using namespace arma;
using std::complex;

namespace Oi {

    void detrend( Mat<double>& x, int p = 1);
    void inverse( Mat<double>& x);
    colvec hamming(int m);

    void convmtx(colvec in, int p, mat& out);
    void covar(colvec in, int p, mat& out );

    class PWelch
    {
        private:
            FFTWest gfft_;
            int segmentLength_;
            double samplingInterval_;
            int overlap_;
            bool calcCSD_;

            cx_cube CSD_;
        public:
            PWelch();
            PWelch(int segmentLength);
            PWelch(int segmentLength, int overlap, bool calcCSD_);
            
            void setSamplingInterval(double sampling);
            void apply(const mat& in, mat& out); 
            cx_cube& getCSD();
    };
    
    class Music
    {
        private:

        public:
//            Music()
    };
    
    class Pisarenko
    {

    };

} // namespace Oi

#endif   // ----- #ifndef OIMATH_INC  -----
