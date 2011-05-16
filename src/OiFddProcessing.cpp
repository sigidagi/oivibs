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

#include    "OiFddProcessing.h"
#include    "OiFileFormat.h"
#include    "OiUniversalFileFormat.h"
#include    <iostream>
#include    <boost/progress.hpp>

using std::conj;

namespace Oi 
{

    FddProcessing::FddProcessing(const string& file) : file_(file), samplingInterval_(1.0), segmentLength_(2048)
    {
        frequencies_.set_size(segmentLength_/2+1);        
    }

    FddProcessing::~FddProcessing()
    {

    }
    
    string FddProcessing::getFileName() const
    {
        return file_;
    }
    
    int FddProcessing::getProcessId() const
    {
        return Process::FDD;
    }


    // 
    void FddProcessing::powerSpectralDensity(const mat& channels)
    {
        //  numbers of rows and cols should be equal to number of channels.
        //  segmentLength represent cube depth where PSD values are stored.
        unsigned int nrows = channels.n_rows;
        unsigned int ncols = channels.n_cols;
        if (nrows == 0 || ncols == 0)
            return;

        PSD_.reset();
        PSD_ = zeros<mat>(segmentLength_/2+1, ncols);
   
        pwelch_.apply(channels, PSD_); 
    }
    
    void FddProcessing::singularValueDecomposition()
    {

        cx_cube& CSD = pwelch_.getCSD();
        if (CSD.is_empty())
            return;

        // form singular values matrix
        // after FFT power spectrum is fliped.
        // CSD_.n_slices == segmentLength_/2+1
        singularValues_.set_size(CSD.n_slices, CSD.n_cols);
        colvec singvalues(CSD.n_cols);
        singularVectors_.set_size(CSD.n_cols, CSD.n_cols, CSD.n_slices);
        cx_mat Uunitary;
        cx_mat Vunitary;

        unsigned int lastSlice = CSD.n_slices-1;
        int idx = (int)lastSlice; 
 
        cout << "-- SVD calculation ---\n";
        boost::progress_display showProgess(idx);
        
        for (unsigned int i = 0; i < CSD.n_slices; ++i) 
        {
            arma::svd(Uunitary, singvalues, Vunitary, CSD.slice(i));
            
            singularValues_.row(i) = trans(singvalues);
            singularVectors_.slice(i) = Uunitary;
            ++showProgess;
        }
        cout << "Done\n\n";
    }

    bool FddProcessing::start(const FileFormatInterface* format)
    {
        if (format == NULL)
            return false;
 
        // search for data, if not exist - return from function.
        const mat& channels = format->getChannels();

        unsigned int nrows = channels.n_rows;
        unsigned int ncols = channels.n_cols;
        if (nrows == 0 || ncols == 0)
        {
            cout << "No data exist!\n";
            return false;
        }

        samplingInterval_ = format->getSamplingInterval();
        if (samplingInterval_  == 1)
            std::cout << "Coundn't get sampling interval. Set value to 1\n";
        
        frequencies_ = arma::linspace<colvec>(0, 1/(2.0*samplingInterval_), segmentLength_/2+1); 

        // calculate PSD using welch method 
        powerSpectralDensity(channels);
        
        // singular values and singular vectors across frequency line.
        singularValueDecomposition();

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
    
    const double* FddProcessing::getSpectralDensity(int& nsamples, int& nchannels) const
    {
        nsamples = PSD_.n_rows;
        nchannels = PSD_.n_cols;
        return PSD_.memptr();
    }

    const arma::cx_cube& FddProcessing::getSingularVectors() const
    {
        return singularVectors_;
    }
    
    arma::cx_mat& FddProcessing::getModes(unsigned int freqIndex)
    {
        if (frequencies_.n_elem < freqIndex)
        {
            modes_.reset();
            return modes_;
        }

        modes_ = singularVectors_.slice(freqIndex);
        return modes_;
    }

} // namespace Oi
