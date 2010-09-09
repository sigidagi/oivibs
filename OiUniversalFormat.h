#ifndef _UNIVERSALFORMAT_H
#define _UNIVERSALFORMAT_H

#include "OiFormat.h"
#include <vector>
#include <fstream>
#include <string>
#include <armadillo>

using std::vector;
using std::string;
using std::streampos;

namespace Oi {

class OiUniversalFormat : public OiFormat
{
	// pivate data
private:
	std::ifstream uffFile;
    
    // channels are represented in columns 
    arma::mat data_;
    
    arma::mat nodes_;
    arma::umat lines_;
    arma::umat surfaces_;

	bool bExistNodes;
    bool bExistLines;
    bool bExistSurfaces;
    bool bExistData;
	int posNodes, posLines, posSurface;
	vector<int> m_vposData;

    vector<double> vSamplingInterval;
    vector<int> vNumberOfSamples;

    double samplingInterval_;
    int numberOfSamples_;
 
	string m_strFile;
    vector<string> __strStatus;

	// private methods
private:
    void searchForSamplingT();
	void searchForData();
	void parseNodes();
	void parseLines();
	void parseSurfaces();
	void parseData(const int pos, int column);

public:
    OiUniversalFormat();
	~OiUniversalFormat();

    void parse(const string file);
    
    bool existNodes();
    bool existLines();
    bool existSurfaces();
    bool existData();

    const arma::mat& getNodes();
    const arma::umat& getLines();
    const arma::umat& getSurfaces();
    const arma::mat& getData();

    double getSamplingT();
    int getNumberOfSamples();
};

} // namespace Oi

#endif
