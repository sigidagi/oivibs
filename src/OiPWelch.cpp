// =====================================================================================
// 
//       Filename:  OiPWelch.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-03-12 21:56:35
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


#include	"OiPWelch.h"
#include	<iostream>
#include	<vector>
#include	<OiMath.h>
#include	<boost/progress.hpp>

using std::cout;
using std::vector;

namespace Oi {

    PWelch::PWelch() : segmentLength_(2048), samplingInterval_(1.0), calcCSD_(true)
    {
        gfft_.setDataLength(segmentLength_);
        overlap_ = std::floor(2*segmentLength_/3);
    }

    PWelch::PWelch(int segmentLength) : segmentLength_(segmentLength), samplingInterval_(1.)
    {
        overlap_ = std::floor(2*segmentLength_/3);
        gfft_.setDataLength(segmentLength_);
    }
 
    PWelch::PWelch(int segmentLength, int overlap) : segmentLength_(segmentLength), overlap_(overlap), samplingInterval_(1.)
    {
        gfft_.setDataLength(segmentLength_);
    }

    PWelch::PWelch(int segmentLength, int overlap, double sampling, bool bCSD) : segmentLength_(segmentLength), overlap_(overlap), samplingInterval_(sampling), calcCSD_(bCSD)
    {
        gfft_.setDataLength(segmentLength_);
    }
   
    void PWelch::setSegmentLength(int length)
    {
        segmentLength_ = length;
    }

    void PWelch::setSamplingInterval(double sampling)
    {
        samplingInterval_ = sampling;
    }
    
    void PWelch::setOverlap(int overlap)
    {
        overlap_ = overlap;
    }
    
    void PWelch::setCalculateCSD(bool calcCSD)
    {
        calcCSD_ = calcCSD;
    }

    void PWelch::apply(const mat& in, mat& psd_out)
    {
        unsigned int nrows = in.n_rows;
        unsigned int ncols = in.n_cols;
        unsigned int j(0), row_pos(0);
        
        if (nrows == 0 || ncols == 0)
            return;

        if (ncols == 1)
            calcCSD_ = false;
        
        if (calcCSD_)
            CSD_ = zeros<cx_cube>(ncols, ncols, segmentLength_/2+1);

        int step = segmentLength_ - overlap_;
        colvec hamming = Oi::hamming(segmentLength_);
        
        vector<int> positions;
        while ((row_pos + segmentLength_) < nrows)
        {
            positions.push_back(row_pos);
            row_pos += step;
        }
        
        psd_out.reset();
        psd_out = zeros<mat>(segmentLength_/2+1, ncols);

        cx_mat tempPSD(segmentLength_/2+1, ncols);

        mat chunk(segmentLength_, ncols);
        vector<int>::iterator it;
 
        cout << "-- PSD and CSD calculation ---\n";
        boost::progress_display showProgess(positions.size());
       
        for( it = positions.begin(); it != positions.end(); ++it)
        {
            chunk = in.rows(*it, *it + segmentLength_-1);
            detrend(chunk);

            for (j = 0; j < ncols; ++j)
            {
                 chunk.col(j) = chunk.col(j) % hamming;
                 gfft_.exec( chunk.colptr(j), tempPSD.colptr(j) );
            }
            
            psd_out += arma::abs(tempPSD);

            if (calcCSD_)
            {
                for (size_t col = 0; col < tempPSD.n_cols; ++col)
                {
                    for (size_t col2 = 0; col2 < tempPSD.n_cols; ++col2)
                    {
                        for (size_t row = 0; row < tempPSD.n_rows; ++row)
                            CSD_(col, col2, row) += tempPSD(row, col)*conj(tempPSD(row, col2));
                    }
                }
            }

            ++showProgess;
        }
        std::cout << "Done!\n\n";

        int nshifts = (positions.end() - positions.begin()); 

        // Scale factor
        // scale = n_ffts * seg_len * Fs * win_meansq
        // where win_meansq = (window.' * window)/seg_len;
        double win_meansq = arma::accu(hamming % hamming)/hamming.n_elem;
        double scale = win_meansq * nshifts * (double)segmentLength_/(2.0*samplingInterval_);
       
        if (calcCSD_)
            CSD_ /= scale;

        psd_out /= scale;
    }

    cx_cube& PWelch::getCSD()
    {
        return CSD_;
    }
} // namespace 
