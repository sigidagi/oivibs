// =====================================================================================
// 
//       Filename:  OiProcessing.h
// 
//    Description: 
// 
//        Version:  1.0
//        Created:  2010-09-07 12:16:18
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

#ifndef _OIPROCESSING_H
#define _OIPROCESSING_H

#include	<string>
#include	<armadillo>
#include    <boost/shared_ptr.hpp>

using boost::shared_ptr;
using std::string;

namespace Oi 
{
    class FileFormatInterface;

    class ProcessingInterface 
    {
        public:
            virtual ~ProcessingInterface();

        public:
            static shared_ptr<ProcessingInterface> createProcess(int processName, const string& file);
            virtual bool start(const FileFormatInterface* format) = 0;
            virtual string getFileName() const = 0;
            virtual int getProcessId() const = 0;
            
            virtual const double* getSpectralDensity(int& nsamples, int& nchannels) const  = 0;
            virtual const double* getSingularValues(int& nrows, int& ncols) const = 0;
            virtual const double* getFrequencies(int& length) const = 0;
            virtual const arma::cx_cube& getSingularVectors() const = 0;
            virtual arma::cx_mat& getModes(unsigned int freqIndex) = 0;
    };

} // namespace Oi

#endif
