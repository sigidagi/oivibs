#ifndef _UNIVERSALFORMAT_H
#define _UNIVERSALFORMAT_H

#include <armadillo>
#include <vector>
#include <fstream>
#include <string>
#include <boost/shared_array.hpp>
#include <mysql++.h>

using std::vector;
using std::string;
using std::streampos;
using boost::shared_array;

class UniversalFormat
{
	// pivate data
private:
	std::ifstream uffFile;

	bool bExistNodes;
    bool bExistLines;
    bool bExistSurfaces;
    bool bExistData;
	int posNodes, posLines, posSurface;
	vector<int> m_vposData;

    // channels are represented in columns 
    arma::mat m_matData;

	string m_strFile;
    vector<string> __strStatus;
    vector<string> __vFiles;

	// private methods
private:
    void searchForSamplingT();
	void searchForData();
	void parseNodes(mysqlpp::Connection& con);
	void parseLines(mysqlpp::Connection& con);
	void parseSurfaces(mysqlpp::Connection& con);
	void parseData(const int pos, int column);


public:
	explicit UniversalFormat(const vector<string>& file);
	~UniversalFormat();

    vector<double> vSamplingInterval;
    vector<int> vNumberOfSamples;
    
    void parse();

    bool existNodes();
    bool existLines();
    bool existSurfaces();
    bool existData();

    const arma::Mat<double>& getData() const;
};



#endif
