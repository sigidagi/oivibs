// =====================================================================================
// 
//       Filename:  OiSsiProcessing.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-13 12:48:02
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

#include    "OiSsiProcessing.h"
#include	"OiUtil.h"

namespace Oi {

    SsiProcessing::SsiProcessing(const string& file) : file_(file)
    {

    }
    
    string SsiProcessing::getFileName() const
    {
        return file_;
    }
    
    int SsiProcessing::getProcessId() const
    {
        return Process::SSI;
    }

    bool SsiProcessing::start(const FileFormatInterface* format)
    {
        assert(false);
        // not implemented jet.
        return false;
    }
    
    const double* SsiProcessing::getSingularValues(int& nrows, int& ncols) const
    {
        nrows = singularValues_.n_rows;
        ncols = singularValues_.n_cols;
        return singularValues_.memptr();        
    }

    const arma::cx_cube& SsiProcessing::getSingularVectors() const
    {
        return singularVectors_;
    }
    
    const double* SsiProcessing::getSpectralDensity(int& nsamples, int& nchannels) const
    {
        return NULL;
    }

    const double* SsiProcessing::getFrequencies(int& length) const
    {
        length = frequencies_.n_elem;
        return frequencies_.memptr();
    }

    arma::cx_mat& SsiProcessing::getModes(unsigned int freqIndex)
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


