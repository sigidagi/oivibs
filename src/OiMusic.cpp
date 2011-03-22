// =====================================================================================
// 
//       Filename:  OiMusic.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-03-12 22:08:17
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


#include	"OiMusic.h"
#include	<iostream>
#include	<vector>
#include	<OiMath.h>
#include	<boost/progress.hpp>

using std::cout;
using std::vector;

namespace Oi {

    Music::Music() : segmentLength_(2048), samplingInterval_(1.0), calcCSD_(true)
    {
        gfft_.setDataLength(segmentLength_);
    }

    Music::Music(int segmentLength) : segmentLength_(segmentLength), samplingInterval_(1.)
    {
        overlap_ = std::floor(2*segmentLength_/3);
        gfft_.setDataLength(segmentLength_);
    }
 
    Music::Music(int segmentLength, double sampling, bool bCSD) : segmentLength_(segmentLength), samplingInterval_(sampling), calcCSD_(bCSD)
    {
        gfft_.setDataLength(segmentLength_);
    }
   
    void Music::setSegmentLength(int length)
    {
        segmentLength_ = length;
    }

    void Music::setSamplingInterval(double sampling)
    {
        samplingInterval_ = sampling;
    }
    
    void Music::setCalculateCSD(bool calcCSD)
    {
        calcCSD_ = calcCSD;
    }

    void Music::apply(const mat& in, mat& out)
    {
/*
 *    void music(Row<double> in, int p, int M, complex<double>* power)
 *    {
 *        if (M < p-1)
 *        {
 *            std::cerr << "Size of R is inappropriate!\n"; 
 *            return;
 *        }
 *        
 *        Mat<T> R;
 *        covar(in, M, R); 
 *
 *        Col<T> eigval;
 *        Mat<T> eigvec;
 *        arma::eig_sym(eigval, eigvec, R);
 *        
 *        uvec indices = sort_index(eigval)
 *        int nfft = in.n_elem;
 *
 *        FFTWest gfft(nfft);
 *        cx_vec out(nfft/2+1);
 *
 *        for (int i = 0; i <= M-p; ++i)
 *        {
 *            gfft.exec( eigvec.colptr( indices(i) out.memptr()) );
 *            power += out; 
 *        }
 *    }
 */

    }

    cx_cube& Music::getCSD()
    {
        return CSD_;
    }
} // namesapce 
