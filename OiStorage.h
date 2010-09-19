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
#include <memory>

using std::auto_ptr;

namespace Oi
{
    class FileFormatInterface;
    class ProcessingInterface;

    class StorageInterface
    {
        public:
            StorageInterface() : fileFormat_(NULL), proc_(NULL)
            {

            }

            virtual ~StorageInterface(){}
        
        // Interfaces
        public:
            virtual void saveNodes(const arma::mat& nodes) = 0;
            virtual void saveLines(const arma::umat& lines) = 0;
            virtual void saveSurfaces(const arma::umat& surfaces) = 0;
            virtual void saveData(const arma::mat& data) = 0;
        
            virtual void saveSingularValues(const arma::mat& values);
            virtual void saveSingularVectors(const arma::cx_mat& vectors);
            
            FileFormatInterface* getFileFormat();
            ProcessingInterface* getProcess();

        protected:
            auto_ptr<FileFormatInterface> fileFormat_;
            auto_ptr<ProcessingInterface> proc_;
    };

}


#endif 
