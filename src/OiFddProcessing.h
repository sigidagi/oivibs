// =====================================================================================
// 
//       Filename:  OiFddProcessing.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-13 09:56:29
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

#ifndef _OIFDDPROCESSING_H
#define _OIFDDPROCESSING_H

#include    "OiProcessing.h" 
#include	"OiPWelch.h"
#include    <armadillo>
#include	<complex>

using namespace arma;
using std::complex;

namespace Oi 
{
    class FileFormatInterface;

    class FddProcessing : public ProcessingInterface 
    {
         public:
            explicit FddProcessing (const string& file);
            ~FddProcessing();
            
            
        // ProcessingInterface
         public:
            bool start(const FileFormatInterface* format);
            string getFileName() const;
            int getProcessId() const;
            
            const double* getSpectralDensity(int& nsamples, int& nchannels) const;
            const double* getSingularValues(int& nrows, int& ncols) const;
            const double* getFrequencies(int& length) const;
            
            arma::cx_mat& getModes(unsigned int freqIndex);
            const arma::cx_cube& getSingularVectors() const;

            
        // private methods 
        private:
            
            void powerSpectralDensity(const mat& channels);
            //void crossSpectralDensity();
            void singularValueDecomposition();

        // private member variables.
        private:
            string file_;   

            PWelch pwelch_; 
            mat PSD_;
            
            arma::Mat<double> singularValues_;
            cx_cube singularVectors_;
            colvec frequencies_;

            double samplingInterval_;
            // segmentLength_ just a duoble value of nfft. if nfft = 1024, then segmentLength_ = 2*1024.
            int segmentLength_;
            cx_mat modes_;

    };

} // namespace Oi

#endif
