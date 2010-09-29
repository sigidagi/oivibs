#ifndef _OIASCIIFILEFORMAT_H
#define _OIASCIIFILEFORMAT_H

#include "OiFileFormat.h"
#include <armadillo>
#include <string>
#include <fstream>

using std::string;

namespace Oi {
    
    class StorageInterface;

    class AsciiFileFormat : public FileFormatInterface  
    {
        public:
            AsciiFileFormat(Root* owner);
            ~AsciiFileFormat(){}

        public:
            virtual void parse(const string& file);
            
            virtual bool existNodes();
            virtual bool existLines();
            virtual bool existSurfaces();
            virtual bool existRecords();

            virtual arma::mat& getNodes();
            virtual arma::umat& getLines();
            virtual arma::umat& getSurfaces();
            virtual arma::mat& getRecords();

            virtual double getSamplingInterval();
            virtual int getNumberOfSamples();

        private:
            std::ifstream fileStream_;
            
           
            bool existData_;
    };

} // namespace Oi

#endif
