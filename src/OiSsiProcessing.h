// =====================================================================================
// 
//       Filename:  OiSsiProcessing.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-13 12:44:48
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
#ifndef _OISSIPROCESSING_H
#define _OISSIPROCESSING_H

#include    "OiProcessing.h"
#include	<cassert>

namespace Oi 
{
    class FileFormatInterface;
    
    class SsiProcessing : public ProcessingInterface 
    {
        private:
            string file_;
            arma::mat singulatValues_;
            arma::cx_cube singulatVectors_;
            arma::colvec frequencies_;

        public:
            explicit SsiProcessing(const string& file);

         public:
            bool start(const FileFormatInterface* format);
            string getFileName() const;
            const double* getSingularValues(int& nrows, int& ncols) const;
            const double* getFrequencies(int& length) const;
            const arma::cx_cube& getSingularVectors() const;
    };

} // namespace Oi

#endif
