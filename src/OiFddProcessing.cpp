// =====================================================================================
// 
//       Filename:  OiFddProcessing.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-05-28 12:00:26
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

#include "OiFddProcessing.h"
#include "OiFileFormat.h"
#include "OiUniversalFileFormat.h"

#include <iostream>
#include <cmath>
#include "gfft/gfft.h"
#include <boost/progress.hpp>

namespace Oi 
{

    FddProcessing::FddProcessing(const string& file) : file_(file)
    {

    }

    FddProcessing::~FddProcessing()
    {

    }
    
    string FddProcessing::getFileName() const
    {
        return file_;
    }
        
    void  FddProcessing::inverse( Mat<double>& x)
    {
        if (x.n_cols == 0 || x.n_rows == 0)
            return;

        arma::pinv(x); 
    }

    // 
    void  FddProcessing::detrend( Mat<double>& x, int p)
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

    void FddProcessing::createPSD(cx_cube& psd, mat& chunk)
    {
        std::complex<double> value1;
        std::complex<double> value2;
        size_t row, col, col2;
        
        for (row = 0; row < chunk.n_rows/2; ++row)
        {
            for (col = 0; col < chunk.n_cols; ++col)
            {
                 //          
                 value1 = complex<double>(chunk(2*row, col), chunk(2*row+1, col));                      
                
                 for (col2 = 0; col2 < chunk.n_cols; ++col2)
                 {
                     value2 = complex<double>(chunk(2*row, col2), chunk(2*row+1, col2));
                     psd(col, col2, row) += value1*std::conj(value2);
                 }
            }
        }

    }

    bool FddProcessing::start(const FileFormatInterface* format)
    {
        if (format == NULL)
            return false;
        
        // search for data, if not exist - return from function.
        const mat& refData = format->getChannels();

        unsigned int nrows = refData.n_rows;
        unsigned int ncols = refData.n_cols;
        if (nrows == 0 || ncols == 0)
            return false;

        // runtime definition of the data length
        // P - power of 2: 2^10 = 1024 samples (after FFT)
        int P = 10;
        // lenght of data before FFT; 
        int segmentLength = 2*(1 << P);
        int overlap = std::floor(segmentLength*2/3);

    // range of the needed GFFT classes
        const unsigned Min = 1;
        const unsigned Max = 27;

    // initialization of the object factory
        Loki::Factory<AbstractFFT<Tp>,unsigned int> gfft_factory;
        FactoryInit<GFFTList<GFFT,Min,Max>::Result>::apply(gfft_factory);

    // create an instance of the GFFT
        AbstractFFT<Tp>* gfft = gfft_factory.CreateObject(P);

    //    mat X(segmentLength, ncols);
    //    cx_mat Y(segmentLength, ncols);
    //
    //  numbers of rows and cols should be equal to number of channels.
    //  segmentLength represent cube depth where PSD values are stored.
        colvec hamming = Oi::hamming(segmentLength);
       
        unsigned int j;
        int row_pos = 0;
        powerSpectrum_ = zeros<cx_cube>(ncols, ncols, segmentLength/2+1);
        mat chunk(segmentLength, ncols);
        int step = segmentLength - overlap;
        unsigned int nslices = 0;

        vector<int> positions;
        while ((row_pos + segmentLength) < nrows)
        {
            positions.push_back(row_pos);
            row_pos += step;
        }
        
        cout << "-- PSD calculation ---\n";
        boost::progress_display showProgess(positions.size());

        vector<int>::iterator it;
        for( it = positions.begin(); it != positions.end(); ++it)
        {
            chunk = refData.rows(*it, *it + segmentLength-1);
            detrend(chunk);
            for (j = 0; j < ncols; ++j)
            {
                chunk.col(j) = chunk.col(j) % hamming;
                gfft->fft(chunk.memptr() + j*segmentLength );
                createPSD(powerSpectrum_, chunk);    
            }
            ++nslices;
            ++showProgess;
        }
        
        cout << "Done.\n";

        double T = format->getSamplingInterval();
        frequencies_ = 1/(2.0*T) * linspace<colvec>(0,1, segmentLength/2); 
        
        // Scale factor
        // scale = n_ffts * seg_len * Fs * win_meansq
        // where win_meansq = (window.' * window)/seg_len;
        double win_meansq = arma::accu(hamming % hamming)/hamming.n_elem;
        double scale = win_meansq * nslices * (double)segmentLength/(2.0*T);
       
        powerSpectrum_ = powerSpectrum_ / scale;
       
        // form singular values matrix
        // after FFT power spectrum is fliped.
        singularValues_.set_size(powerSpectrum_.n_slices, ncols);
        colvec singvalues(ncols);
        singularVectors_.set_size(ncols, ncols, powerSpectrum_.n_slices);
        cx_mat Uunitary;
        cx_mat Vunitary;

        unsigned int lastSlice = powerSpectrum_.n_slices-1;
        int idx = (int)lastSlice; 
        
        while (idx != -1)       
        {
            arma::svd(Uunitary, singvalues, Vunitary, powerSpectrum_.slice(idx));
            singularValues_.row(lastSlice-idx) = trans(singvalues);
            singularVectors_.slice(lastSlice-idx) = Uunitary;
            --idx;
        }

        return true;

    }


    const double* FddProcessing::getSingularValues(int& nrows, int& ncols) const
    {
        nrows = singularValues_.n_rows;
        ncols = singularValues_.n_cols;
        return singularValues_.memptr();
    }
        
    const double* FddProcessing::getFrequencies(int& length) const
    {
        length = frequencies_.n_elem;
        return frequencies_.memptr();
    }

    const arma::cx_cube& FddProcessing::getSingularVectors() const
    {
        return singularVectors_;
    }

} // namespace Oi
