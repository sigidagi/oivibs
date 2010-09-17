#ifndef _UNIVERSALFORMAT_H
#define _UNIVERSALFORMAT_H

#include "OiStorage.h"
#include "OiFileFormat.h"
#include <vector>
#include <fstream>
#include <string>
#include <armadillo>

using std::vector;
using std::string;
using std::streampos;

namespace Oi {

    class UniversalFileFormat : public FileFormatInterface
    {
        // pivate data
    private:
        std::ifstream fileStream_;
        
        // channels are represented in columns 
        arma::mat data_;
        arma::mat nodes_;
        arma::umat lines_;
        arma::umat surfaces_;

        bool existNodes_;
        bool existLines_;
        bool existSurfaces_;
        bool existData_;
        int nodePosition_, linePosition_, surfacePosition_;
        vector<int> dataPositionList_;

        double samplingInterval_;
        int numberOfSamples_;
     

        // private methods
    private:
        void searchForSamplingT();
        void searchForData();
        void parseNodes();
        void parseLines();
        void parseSurfaces();
        void parseData(const int pos, int column);

    public:
        UniversalFileFormat(StorageInterface* storage);
        ~UniversalFileFormat();

        void parse(const string& file);

        bool existNodes();
        bool existLines();
        bool existSurfaces();
        bool existData();

        const arma::mat& getNodes();
        const arma::umat& getLines();
        const arma::umat& getSurfaces();
        const arma::mat& getData();

        double getSamplingInterval();
        int getNumberOfSamples();
    };

} // namespace Oi

#endif
