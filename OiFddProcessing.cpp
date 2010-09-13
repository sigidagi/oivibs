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
//        Company:  sigidagis
// 
// =====================================================================================

#include "OiFddProcessing.h"
#include "OiFileFormat.h"
#include "OiUniversalFileFormat.h"

#include <iostream>
#include <cmath>
#include "gfft/gfft.h"

namespace Oi 
{
    void  FddProcessing::inverse( Mat<double>& x)
    {
        if (x.n_cols == 0 || x.n_rows == 0)
            return;

        pinv(x); 
    }

    // 
    void  FddProcessing::detrend( Mat<double>& x, int p)
    {
        if (x.n_cols == 0 || x.n_rows == 0)
            return;
        

        if (x.n_rows == 1)
            x = trans(x);
       
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
        x = x - b *( pinv(b)*x );
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

    void FddProcessing::createPSD(cube& psd, mat& chunk)
    {
        double a, b, c, d;
        int row, col, col2;
        for (row = 0; row < (int)chunk.n_rows/2; ++row)
        {
            for (col = 0; col < (int)chunk.n_cols; ++col)
            {
                 a = chunk(2*row, col);
                 b = chunk(2*row+1, col);

                 for (col2 = 0; col2 < (int)chunk.n_cols; ++col2)
                 {
                     c = chunk(2*row, col2);
                     d = chunk(2*row+1, col2);

                     psd(col, col2, row) += sqrt(pow((a*c - b*d),2) + pow((b*c + a*d),2));
                 }
            }
        }

    }

    bool FddProcessing::start(FileFormatInterface* fileFormat)
    {
        const mat& refData = fileFormat->getData();
        int nrows = refData.n_rows;
        int ncols = refData.n_cols;
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
       
        int nslices = 0;
        int nn = 0;
        int row_pos = 0;
        powerSpectrum_ = zeros<cube>(ncols, ncols, segmentLength/2);
        mat chunk(segmentLength, ncols);

        while (row_pos + segmentLength < nrows)
        {
            chunk = refData.rows(row_pos, row_pos + segmentLength-1);
            detrend(chunk);
            for (nn = 0; nn < ncols; ++nn)
            {
                chunk.col(nn) = chunk.col(nn) % hamming;
                gfft->fft(chunk.memptr() + nn*segmentLength );
           
                createPSD(powerSpectrum_, chunk);    
            }

            ++nslices; 
            std::cout << nslices << "\n";
            row_pos += segmentLength - overlap; 
        }
        

        double T = fileFormat->getSamplingInterval();
        frequencies_ = 1/(2.0*T) * linspace<colvec>(0,1, segmentLength/2); 

        
        // Scale factor
        // scale = n_ffts * seg_len * Fs * win_meansq
        // where win_meansq = (window.' * window)/seg_len;

        double win_meansq = arma::accu(hamming % hamming)/hamming.n_elem;
        double scale = win_meansq * nslices * (double)segmentLength/(2.0*T);
       
        powerSpectrum_ = powerSpectrum_ / scale;

        singularValues_.set_size(ncols, powerSpectrum_.n_slices);
        colvec singvalues(ncols);
        for (nn = 0; nn < (int)powerSpectrum_.n_slices; ++nn)
        {
            arma::svd(singvalues, powerSpectrum_.slice(nn)); 
            singularValues_.col(nn) = singvalues;
        }

        rowvec svd0(singularValues_.n_cols);
        svd0 = singularValues_.row(0);
        svd0.save("SVD.txt", arma_ascii);
        
        return true;

    }

} // namespace Oi
