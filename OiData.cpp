// =====================================================================================
// 
//       Filename:  OiData.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-06-06 21:36:32
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Sigitas Dagilis (), sigidagi@gmail.com
//        Company:  sigidagis
// 
// =====================================================================================

#include "OiData.h"
#include "SignalProcessing.h"
#include "gfft/gfft.h"

using namespace arma;

namespace Oi {

/*
 *OiData& OiData::Instance()
 *{
 *    static OiData thisOiData;
 *    return thisOiData;
 *}
 *
 */

void OiData::createPSD(cube& psd, mat& chunk)
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

bool OiData::startProcessing()
{
    const mat& refData = getData();
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
        Oi::detrend(chunk);
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
    
    double T;
    if (vSamplingInterval.empty())
        T = 1.0;    

    T = vSamplingInterval[0];
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

arma::Mat<double>& OiData::getData() 
{
    return m_matData;
}

} // namespace Oi
