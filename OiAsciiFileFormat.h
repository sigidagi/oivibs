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
            AsciiFileFormat(StorageInterface* storage);
            ~AsciiFileFormat(){}

        public:
            virtual void parse(const string& file);
            
            virtual bool existNodes();
            virtual bool existLines();
            virtual bool existSurfaces();
            virtual bool existData();

            virtual const arma::mat& getNodes();
            virtual const arma::umat& getLines();
            virtual const arma::umat& getSurfaces();
            virtual const arma::mat& getData();

            virtual double getSamplingInterval();
            virtual int getNumberOfSamples();

        private:
            
            std::ifstream fileStream_;

            bool existData_;
    };

} // namespace Oi

#endif
