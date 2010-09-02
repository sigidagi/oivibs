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
#include "SignalProcessing.h"
//#include <acml.h>
#include <fftw3.h>
#include "gfft/gfft.h"
#include "UniversalFormat.h"

namespace Oi {

    void  inverse( Mat<double>& x)
    {
        if (x.n_cols == 0 || x.n_rows == 0)
            return;

        pinv(x); 
    }

    // 
    void  detrend( Mat<double>& x, int p)
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
