// =====================================================================================
// 
//       Filename:  SignalProcessing.cpp
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

#include <iostream>
#include <cmath>
#include "OiProcessing.h"
#include "gfft/gfft.h"
#include "OiUniversalFormat.h"

//#include <acml.h>
//#include <fftw3.h>

namespace Oi {

    void  OiProcessing::inverse( Mat<double>& x)
    {
        if (x.n_cols == 0 || x.n_rows == 0)
            return;

        pinv(x); 
    }

    // 
    void  OiProcessing::detrend( Mat<double>& x, int p)
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

    void OiProcessing::createPSD(cube& psd, mat& chunk)
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

    bool OiProcessing::start(OiFormat* pFormat)
    {
        const mat& refData = pFormat->getData();
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
        m_Pxx = zeros<cube>(ncols, ncols, segmentLength/2);
        mat chunk(segmentLength, ncols);

        while (row_pos + segmentLength < nrows)
        {
            chunk = refData.rows(row_pos, row_pos + segmentLength-1);
            detrend(chunk);
            for (nn = 0; nn < ncols; ++nn)
            {
                chunk.col(nn) = chunk.col(nn) % hamming;
                gfft->fft(chunk.memptr() + nn*segmentLength );
           
                createPSD(m_Pxx, chunk);    
            }

            ++nslices; 
            std::cout << nslices << "\n";
            row_pos += segmentLength - overlap; 
        }
        

        double T = pFormat->getSamplingT();
        m_Freq = 1/(2.0*T) * linspace<colvec>(0,1, segmentLength/2); 

        
        // Scale factor
        // scale = n_ffts * seg_len * Fs * win_meansq
        // where win_meansq = (window.' * window)/seg_len;

        double win_meansq = arma::accu(hamming % hamming)/hamming.n_elem;
        double scale = win_meansq * nslices * (double)segmentLength/(2.0*T);
       
        m_Pxx = m_Pxx / scale;

        m_SVD.set_size(ncols, m_Pxx.n_slices);
        colvec singvalues(ncols);
        for (nn = 0; nn < (int)m_Pxx.n_slices; ++nn)
        {
            arma::svd(singvalues, m_Pxx.slice(nn)); 
            m_SVD.col(nn) = singvalues;
        }

        rowvec svd0(m_SVD.n_cols);
        svd0 = m_SVD.row(0);
        svd0.save("SVD.txt", arma_ascii);
        
        return true;

    }

    // changes time sequecies (real value ) to power spectral values.
/*
 *    void  fftw(Mat<double>& x)
 *    {
 *         fftw_complex *in, *out;
 *         fftw_plan p;
 *
 *         int N = (int)x.n_rows;
 *
 *         in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
 *         out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
 *         p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
 *       
 *         int i; 
 *         std::complex<double> temp;
 *
 *         for (int j = 0; j < (int)x.n_cols; ++j)
 *         {
 *             for (i = 0; i < N; ++i)
 *             {
 *                 in[i][0] =  x(i, j);
 *                 in[i][1] = 0.0;
 *             }
 *            
 *             fftw_execute(p); [> repeat as needed <]
 *
 *             for(i = 0; i < N; ++i)
 *             {
 *                // y(i, j) = std::complex<double>(out[i][0], out[i][1]); 
 *                temp = std::complex<double>(out[i][0], out[i][1]);
 *                temp /= x.n_rows;
 *                x(i,j) = std::abs( std::pow(temp, 2) );
 *             }
 *         }
 *        
 *         fftw_destroy_plan(p);
 *         fftw_free(in); fftw_free(out);
 *    }
 */

/*
 *    void  ffta(double* px, unsigned long nelements)
 *    {
 *
 *        int info;
 *        double* comm = (double *)malloc((3*nelements+100)*sizeof(double));
 *
 *     //  initialize communication work array
 *        dzfft(0,nelements, px, comm, &info);
 *
 *        // compute real -> Hermitian transform
 *        dzfft(1,nelements, px, comm, &info);
 *
 *        free(comm);
 *
 *
 *    }
 */

/*
 *    void four1(double* data, unsigned long nn)
 *    {
 *        unsigned long n, mmax, m, j, istep, i;
 *        double wtemp, wr, wpr, wpi, wi, theta;
 *        double tempr, tempi;
 *
 *        if (nn < 2 || nn & (nn-1))
 *            throw ("in this function second argument \"nn\" must be power of 2.");
 *    
 *        // reverse-binary reindexing
 *        n = nn<<1;
 *        j=1;
 *        for (i=1; i<n; i+=2) {
 *            if (j>i) {
 *                swap(data[j-1], data[i-1]);
 *                swap(data[j], data[i]);
 *            }
 *            m = nn;
 *            while (m>=2 && j>m) {
 *                j -= m;
 *                m >>= 1;
 *            }
 *            j += m;
 *        };
 *
 *        // here begins the Danielson-Lanczos section
 *        mmax=2;
 *        while (n>mmax) {
 *            istep = mmax<<1;
 *            theta = -(2*M_PI/mmax);
 *            wtemp = sin(0.5*theta);
 *            wpr = -2.0*wtemp*wtemp;
 *            wpi = sin(theta);
 *            wr = 1.0;
 *            wi = 0.0;
 *            for (m=1; m < mmax; m += 2) {
 *                for (i=m; i <= n; i += istep) {
 *                    j=i+mmax;
 *                    tempr = wr*data[j-1] - wi*data[j];
 *                    tempi = wr * data[j] + wi*data[j-1];
 *
 *                    data[j-1] = data[i-1] - tempr;
 *                    data[j] = data[i] - tempi;
 *                    data[i-1] += tempr;
 *                    data[i] += tempi;
 *                }
 *                wtemp=wr;
 *                wr += wr*wpr - wi*wpi;
 *                wi += wi*wpr + wtemp*wpi;
 *            }
 *            mmax=istep;
 *        }
 *
 *    }
 */


} // namespace Oi
