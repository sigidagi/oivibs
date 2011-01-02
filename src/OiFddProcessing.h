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

#include "OiProcessing.h" 
#include <armadillo>

using namespace arma;

namespace Oi 
{
    class FileFormatInterface;

    class FddProcessing : public ProcessingInterface 
    {
         public:
            FddProcessing ();
            ~FddProcessing();
            
            const arma::mat& getSingularValues();
            const arma::cx_mat& getSingularVectors();
            
        // ProcessingInterface
         public:
            bool start(const FileFormatInterface* format);
            
        // private methods 
        private:
            void createPSD(cx_cube& psd, mat& chunk);
            void detrend( Mat<double>& x, int p = 1);
            colvec hamming(int m);
            
            // pseudo inverse using SVD
            void inverse( Mat<double>& x );

        // private member variables.
        private:
            cx_cube powerSpectrum_;
            mat singularValues_;
            cx_mat singularVectors_;
            colvec frequencies_;

    };

} // namespace Oi

#endif
