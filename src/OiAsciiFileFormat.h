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
            void parse(const string& file);
            
            bool existNodes();
            bool existLines();
            bool existSurfaces();
            bool existRecords();

            arma::mat& getNodes();
            arma::umat& getLines();
            arma::umat& getSurfaces();
            arma::mat& getRecords();

            double getSamplingInterval();
            int getNumberOfSamples();
    
        private:
            std::ifstream fileStream_;
           
            bool existData_;
    };

} // namespace Oi

#endif
