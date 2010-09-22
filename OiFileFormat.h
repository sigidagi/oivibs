// =====================================================================================
// 
//       Filename:  OiFormat.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-09 13:30:50
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Sigitas Dagilis (), sigidagi@gmail.com
//        Company:  sigidagis
// 
// =====================================================================================

#ifndef _OIFORMAT_H
#define _OIFORMAT_H

#include <armadillo>
#include <string>
#include <memory>

using std::auto_ptr;
using std::string;

namespace Oi {
    
    class StorageInterface;

    class FileFormatInterface
    {
        public:
            FileFormatInterface(StorageInterface* storage);
            virtual ~FileFormatInterface(){}

        public:
            static auto_ptr<FileFormatInterface> createFileFormat(StorageInterface* owner, const string& file);

            virtual void parse(const string& file) = 0;
            
            virtual bool existNodes() = 0;
            virtual bool existLines() = 0;
            virtual bool existSurfaces() = 0;
            virtual bool existRecords() = 0;

            virtual arma::mat& getNodes() = 0;
            virtual arma::umat& getLines() = 0;
            virtual arma::umat& getSurfaces() = 0;
            virtual arma::mat& getRecords() = 0;

            double getSamplingInterval();
            int getNumberOfSamples();
            
            StorageInterface* getStorage();
            void setStorage(StorageInterface* storage);

        protected:
            StorageInterface* storage_;
            string file_; 
        
            arma::mat nodes_;
            arma::umat lines_;
            arma::umat surfaces_;
            arma::mat records_;
            
            
            double samplingInterval_;
            int numberOfSamples_;
    };

} // namespace Oi

#endif
