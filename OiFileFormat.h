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


#include	"OiPersist.h"
#include    <armadillo>
#include    <string>
#include    <boost/shared_ptr.hpp>

using boost::shared_ptr;
using std::string;

namespace Oi {
    
    class Root;

    class FileFormatInterface : public PersistInterface
    {
        public:
            FileFormatInterface(Root* owner);
            virtual ~FileFormatInterface(){}

        public:
            static shared_ptr<FileFormatInterface> createFileFormat(Root* owner, const string& file);

            virtual void parse(const string& file) = 0;
            
            virtual bool existNodes() = 0;
            virtual bool existLines() = 0;
            virtual bool existSurfaces() = 0;
            virtual bool existRecords() = 0;

            virtual arma::mat& getNodes() = 0;
            virtual arma::umat& getLines() = 0;
            virtual arma::umat& getSurfaces() = 0;
            virtual arma::mat& getRecords() = 0;

            void save(const string& name);
            void load(const string& name);

            double getSamplingInterval();
            int getNumberOfSamples();
            
        protected:
            Root* root_;
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
