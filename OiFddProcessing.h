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
//        Company:  sigidagis
// 
// =====================================================================================

#ifndef _OIFDDPROCESSING_H
#define _OIFDDPROCESSING_H

#include "OiProcessing.h" 
#include <armadillo>

using namespace arma;

namespace Oi 
{
    class Root;

    class FddProcessing : public ProcessingInterface 
    {
         public:
            FddProcessing (Root* owner);
            ~FddProcessing();
            
            const arma::mat& getSingularValues();
            const arma::cx_mat& getSingularVectors();
            
        // ProcessingInterface
         public:
            bool start();
            
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
