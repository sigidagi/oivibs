// =====================================================================================
// 
//       Filename:  OiStorage.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-17 08:38:28
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Sigitas Dagilis (), sigidagi@gmail.com
//        Company:  sigidagis
// 
// =====================================================================================

#ifndef _OISTORAGE_H
#define _OISTORAGE_H

#include <armadillo>

namespace Oi
{
    class StorageInterface
    {
        public:
            virtual ~StorageInterface(){}
        
        // Interfaces
        public:
            virtual void saveNodes(const arma::mat& nodes) = 0;
            virtual void saveLines(const arma::umat& lines) = 0;
            virtual void saveSurfaces(const arma::umat& surfaces) = 0;
            virtual void saveData(const arma::mat& data) = 0;


    };

}


#endif 
