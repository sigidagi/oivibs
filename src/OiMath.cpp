// =====================================================================================
// 
//       Filename:  OiMath.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-03-02 12:05:02
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


#include	"OiMath.h"
#include	<OiFFTWest.h>
#include    <cmath>
#include    <iostream>
#include    <boost/progress.hpp>


namespace Oi {
 
    void  detrend( Mat<double>& x, int p)
    {
        if (x.n_cols == 0 || x.n_rows == 0)
            return;

        if (x.n_rows == 1)
            x = arma::trans(x);
       
       // create a matrix b where columns represent polinomial order:
       // second column - linear approximation of data
       // third column - second order polinomial approximation of data.
       // and so on..
       // number of rows size of data. 
        int rows = x.n_rows;
        mat b = (linspace<colvec>(1,rows,rows) * ones<rowvec>(p+1));

        for (int i = 0; i < (int)b.n_cols; ++i)
        {
            b.col(0) = pow(b.col(0), 0);
        }
        
        // subtract approximation curve from original data.
        x = x - b *( arma::pinv(b)*x );
    }

    colvec  hamming(int m)
    {
        colvec ham;
        if ( m <= 0)
           return ham;

        ham.set_size(m);
        ham = 0.54 - 0.46 * cos( 2 * M_PI * linspace<colvec>(0,m-1, m)/ (m-1) );

        return ham;
    }
   
   void inverse( Mat<double>& x)
    {
        if (x.n_cols == 0 || x.n_rows == 0)
            return;

        arma::pinv(x); 
    }
    
    void convmtx(colvec in, int p, mat& out)
    {
        in.set_size( in.n_elem + p-1 );
        for (int i = 0; i < p-1; ++i)
            in(in.n_elem-1 - i) = 0;
        
        rowvec temp(p);
        temp.zeros();
        temp(0) = in(0);

        out = arma::toeplitz(in, temp);
    }
    
    void covar(colvec in, int p, mat& out )
    {
        unsigned int length = in.n_elem;
        colvec temp = ones<colvec>(length, 1);

        in = in - temp * (arma::sum(in)/length); 
        
        mat out1;
        convmtx(in, p, out1);
        convmtx(in, p, out);
        
        out = trans(out1)*out/(length-1); 
    }

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
    PWelch::PWelch()
    {
        segmentLength_ = 2048;
        gfft_.setDataLength(segmentLength_);
        overlap_ = std::floor(2*segmentLength_/3);
        calcCSD_ = true;
        samplingInterval_ = 1.;        

    }

    PWelch::PWelch(int segmentLength) : segmentLength_(segmentLength)
    {
        overlap_ = std::floor(2*segmentLength_/3);
        gfft_.setDataLength(segmentLength_);
    }
    
    PWelch::PWelch(int segmentLength, int overlap, bool bCSD) : segmentLength_(segmentLength), overlap_(overlap), calcCSD_(bCSD)
    {
        gfft_.setDataLength(segmentLength_);
    }
    
    void PWelch::setSamplingInterval(double sampling)
    {
        samplingInterval_ = sampling;
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
    
} // namespace Oi 
