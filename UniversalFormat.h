#ifndef _UNIVERSALFORMAT_H
#define _UNIVERSALFORMAT_H

#include <vector>
#include <fstream>
#include <string>
#include "OiData.h" 

using std::vector;
using std::string;
using std::streampos;

namespace Oi {

class UniversalFormat
{
	// pivate data
private:
	std::ifstream uffFile;
    
    OiData m_Data;

    arma::mat nodes_;
    arma::umat lines_;
    arma::umat surfaces_;

	bool bExistNodes;
    bool bExistLines;
    bool bExistSurfaces;
    bool bExistData;
	int posNodes, posLines, posSurface;
	vector<int> m_vposData;

	string m_strFile;
    vector<string> __strStatus;
    vector<string> __vFiles;

	// private methods
private:
    void searchForSamplingT();
	void searchForData();
	void parseNodes();
	void parseLines();
	void parseSurfaces();
	void parseData(const int pos, int column);

public:
    UniversalFormat();
	~UniversalFormat();

    void parse(const string file);
    
    bool existNodes();
    bool existLines();
    bool existSurfaces();
    bool existData();

    const arma::mat& getNodes();
    const arma::umat& getLines();
    const arma::umat& getSurfaces();
};

} // namespace Oi

#endif
