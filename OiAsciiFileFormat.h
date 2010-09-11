#ifndef _OIASCIIFILEFORMAT_H
#define _OIASCIIFILEFORMAT_H

#include "OiFileFormat.h"
#include <armadillo>
#include <string>

using std::string;

namespace Oi {

    class AsciiFileFormat : public FileFormatInterface  
    {
        public:
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

            virtual double getSamplingT();
            virtual int getNumberOfSamples();

        private:
            
            arma::mat nodes_;
            arma::umat lines_;
            arma::umat surfaces_;
            arma::mat data_;

            double samplingInterval_;
            int numberOfSamples_;
    };

} // namespace Oi

#endif
