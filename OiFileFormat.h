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

using std::string;

namespace Oi {

    class FileFormatInterface
    {
        public:
            virtual ~FileFormatInterface(){}

        public:
            static FileFormatInterface* createFileFormat(const string& file);

            virtual void parse(const string& file) = 0;
            
            virtual bool existNodes() = 0;
            virtual bool existLines() = 0;
            virtual bool existSurfaces() = 0;
            virtual bool existData() = 0;

            virtual const arma::mat& getNodes() = 0;
            virtual const arma::umat& getLines() = 0;
            virtual const arma::umat& getSurfaces() = 0;
            virtual const arma::mat& getData() = 0;

            virtual double getSamplingT() = 0;
            virtual int getNumberOfSamples() = 0;
           
    };

} // namespace Oi

#endif
