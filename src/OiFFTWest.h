// =====================================================================================
// 
//       Filename:  OiFFT.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-02-28 11:18:37
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


#ifndef  OIFFT_INC
#define  OIFFT_INC

#include	<complex>
#include	<fftw3.h>
#include	<vector>
#include	<algorithm>

using std::complex;
using std::vector;

namespace Oi {

    class FFTWest
    {
        private:
            fftw_plan plan;
            unsigned int dataLength_;
            vector<double> space_;
            
            void init()
            {
                space_.resize(dataLength_); 
            }
        
            
            void exec_r2c(double* in, fftw_complex* out)
            {
                plan = fftw_plan_dft_r2c_1d(dataLength_, in, out, FFTW_ESTIMATE);
                fftw_execute(plan);
                fftw_destroy_plan(plan);
            }

/*
 *            void exec_r2r(double* in, complex<double>* out)
 *            {
 *                plan = fftw_plan_r2r_1d(dataLength_, in, &space_[0], FFTW_R2HC, FFTW_ESTIMATE);
 *                fftw_execute(plan);
 *                fftw_destroy_plan(plan);
 *                
 *                out[0] = complex<double>(space_[0], 0);
 *                out[dataLength_/2] = complex<double>(space_[dataLength_/2], 0); 
 *
 *                for (unsigned int i = 1; i < dataLength_/2; ++i)
 *                {
 *                    out[i] = complex<double>(space_[i], space_[dataLength_-1-i]);
 *                }
 *            }
 */

        public:
            FFTWest() : dataLength_(2048) { init(); }
            FFTWest(unsigned int n) : dataLength_(n) { init(); } 
            
            void setDataLength(int length) { dataLength_ = length; }

            void exec(double* in, complex<double>* out)
            {
                fftw_complex* pfftw = reinterpret_cast<fftw_complex*>(out);
                exec_r2c(in, pfftw);
            }

            void exec(unsigned int in_samples, double* in, complex<double>* out)
            {
                fftw_complex* pfftw = reinterpret_cast<fftw_complex*>(out);
                if (in_samples < dataLength_)
                {
                    std::copy(in, in+in_samples, space_.begin());
                    exec_r2c(&space_[0], pfftw); 
                }
                else
                    exec_r2c(in, pfftw);
            }
    };

} // namespace Oi

#endif   // ----- #ifndef OIFFT_INC  -----
