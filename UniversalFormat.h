#ifndef _UNIVERSALFORMAT_H
#define _UNIVERSALFORMAT_H

#include <armadillo>
#include <vector>
#include <fstream>
#include <string>
#include <boost/shared_array.hpp>
#include <mysql++.h>
#include "OiData.h" 


using std::vector;
using std::string;
using std::streampos;
using boost::shared_array;

class UniversalFormat
{
	// pivate data
private:
	std::ifstream uffFile;
    
    OiData m_Data;

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
	void parseNodes(mysqlpp::Connection& con);
	void parseLines(mysqlpp::Connection& con);
	void parseSurfaces(mysqlpp::Connection& con);
	void parseData(const int pos, int column);

    void parse();

public:
	explicit UniversalFormat(const vector<string>& file);
	~UniversalFormat();

   

    bool existNodes();
    bool existLines();
    bool existSurfaces();
    bool existData();

};



#endif
