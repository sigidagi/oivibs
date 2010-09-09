#include "OiUniversalFormat.h"
#include "OiUtil.h"

#include <algorithm>
#include <sstream>
#include <unistd.h>

using namespace std;

namespace Oi {

OiUniversalFormat::OiUniversalFormat() : bExistNodes(false), bExistLines(false), bExistSurfaces(false), bExistData(false)
{
    
}

void OiUniversalFormat::parse(const string file)
{
	if (file.empty())
		return;

    string strExtension = Oi::stripToExtension(file);

    bool braw_data = false;
    if (strExtension == "txt")
    {
        bool status = data_.load(file, arma::raw_ascii);            
        if (status == false)
        {
            // save into status and return 
            return;
        }
        bExistData = true;
        braw_data = true;
        return;
    }

    string path = Oi::stripToPath(file);
    chdir(path.c_str());

    string fileName = Oi::stripToFileName(file);

    uffFile.open(fileName.c_str(), ios::in);
    if (uffFile.is_open())
    {
        if (strExtension == "cfg")
        {
            searchForSamplingT();
        }
        else if (strExtension == "uff")
        {
            // serches for nodes, lines, surfaces and data in UFF file.
            searchForData();
        }
        else
        {
            // notify that program do not support such file extension.
             return;
        }

        if (bExistNodes)
        {
            parseNodes();
        }
        if (bExistLines)
        {
            parseLines();
        }
        if (bExistSurfaces)
        {
            parseSurfaces();
        }
        if (bExistData)
        {
            for (size_t t = 0; t < m_vposData.size(); ++t)
                parseData(m_vposData[t], t);
             
             
          //  m_Data.getData().save(strBaseName + ".mat");     
        }
        
        uffFile.close();
    }

}

OiUniversalFormat::~OiUniversalFormat()
{

}

void OiUniversalFormat::parseNodes()
{
	if (!bExistNodes)
		return;

    nodes_.reset();
	uffFile.clear();
	uffFile.seekg(posNodes, ios::beg);

	int nodeNumber, temp; 
	string line;
	stringstream ss;
    double x, y, z;     
	
    while (!uffFile.eof())
	{
		getline(uffFile, line);
		ss.str("");
		ss.clear();
		ss << line;
		ss >> nodeNumber;
		if (nodeNumber == -1)
			break;
    
		ss >> temp >> temp >> temp;
		ss >> x >> y >> z;
       
        nodes_ << nodeNumber << x << y << z << arma::endr;
	}
    
}

void OiUniversalFormat::parseLines()
{
	if (!bExistLines)
		return;

    lines_.reset();    
    uffFile.clear();
	uffFile.seekg(posLines, ios::beg);

	int temp, nNumberOfEntries, nTraceLineNumber, nColor;
	string line;
	stringstream ss;
	vector<int> vTempNodes;

	getline(uffFile, line);
	ss << line;
	ss >> nTraceLineNumber >> nNumberOfEntries >> nColor;

	if (nNumberOfEntries == 0)
		return;

	while(!uffFile.eof())
	{
		getline(uffFile, line);
		ss.str("");
		ss.clear();
		ss << line;

		while (ss >> temp)
		{
			if (temp == -1)
				break;

			vTempNodes.push_back(temp);		
		}
		if (temp == -1)
			break;
	}

	if ((int)vTempNodes.size() != nNumberOfEntries)
		return;

	vector<int>::iterator it;
    it = std::remove(vTempNodes.begin(), vTempNodes.end(), 0);
	if (it == vTempNodes.end())
		return;

	vTempNodes.erase(it, vTempNodes.end());

	for (size_t i = 0; i < vTempNodes.size(); i+=2)
	{
        lines_ << (i/2+1) << vTempNodes[i] << vTempNodes[i+1] << arma::endr;
	}
	
	//catch ( std::out_of_range outOfRange )
	//{
	//	std::cout << "\n\nExeption: " << outOfRange.what();
	//	return;
	//}

	//bExistLines = false;
}

void OiUniversalFormat::parseSurfaces()
{
	if (!bExistSurfaces)
		return;
    
    surfaces_.reset();   
	uffFile.clear();
	uffFile.seekg(posSurface, ios::beg);

	string line;
	stringstream ss;
	int SurfaceNumber, FEDescriptor, PhysicalPropNumber, MaterialPropNumber, Color, NumberOfElementNodes;

	while (!uffFile.eof())
	{
		getline(uffFile, line);
		ss << line;
		ss >> SurfaceNumber;
		if (SurfaceNumber == -1)
			break;

		ss >> FEDescriptor >> PhysicalPropNumber >> MaterialPropNumber >> Color >> NumberOfElementNodes;
		
		if ( FEDescriptor != 91 ) // Only thin shell triangular elements allowed
			return;

		if ( NumberOfElementNodes != 3 ) // There need to be three nodes
			return;

		getline(uffFile, line);
		ss.str("");
		ss.clear();
		ss << line;

        int node1, node2, node3;
		ss >> node1 >>  node2 >> node3; 
        
        surfaces_ << node1 << node2 << node3 << arma::endr; 
        
	}
}

void OiUniversalFormat::parseData(const int pos, int column )
{
	if (!bExistData)
		return;

	uffFile.clear();
	uffFile.seekg(pos, ios::beg);

	string line;
	stringstream ss;
	double dValue;

	getline(uffFile, line);
	getline(uffFile, line);
	getline(uffFile, line);
	getline(uffFile, line);
	getline(uffFile, line);
	getline(uffFile, line);
	getline(uffFile, line);

	int nType, nNumberOfPoints, nTemp;
    double dSamplingInterval, dTemp;
	ss << line;
	ss >> nType >> nNumberOfPoints >> nTemp >> dTemp >> dSamplingInterval;
	ss.str("");
	ss.clear();

    // save sampling intervals and number of points for each channel in a vectors
    vSamplingInterval.push_back(dSamplingInterval);
    vNumberOfSamples.push_back(nNumberOfPoints);

    samplingInterval_ = vSamplingInterval[0];
    if (samplingInterval_ != dSamplingInterval)
    {
        // ERROR alert that sampling interval differs in data channels!!
    }
    
    vector<int>::iterator it;
    it = std::min_element(vNumberOfSamples.begin(), vNumberOfSamples.end());
    if (it != vNumberOfSamples.end())
        numberOfSamples_ = *it;
      
    // resize data channels without distroing values inside.   
    data_.set_size( numberOfSamples_, (int)m_vposData.size() );
	
    getline(uffFile, line);
	getline(uffFile, line);
	getline(uffFile, line);
	getline(uffFile, line);

	int nCount = 0;

	while (!uffFile.eof())
	{
		getline(uffFile, line);
		ss << line;
		ss >> dValue;
		if (dValue == -1.0 || nCount == nNumberOfPoints )
			break;

        do 
        {
            data_(nCount, column) = dValue;
            ++nCount;
        }
		while (ss >> dValue);
	
		ss.str("");
		ss.clear();
	}
}

void OiUniversalFormat::searchForSamplingT()
{
	// return stream to the beginning.
	uffFile.seekg(0, ios::beg);

	string line;
    stringstream ss;
    double dT = 0.0;

	while (!uffFile.eof())
	{
		getline(uffFile, line);
		if (uffFile.eof())
			break;

        ss << line;
        if (line == "T\r" && !uffFile.eof())
        {
            getline(uffFile, line);
            ss.str("");
            ss.clear();
            ss << line;
            ss >> dT;
            break;
        }
        
    }

    if (dT != 0.0)
        vSamplingInterval.push_back(dT);       

}

void OiUniversalFormat::searchForData()
{
	// return stream to the beginning.
	uffFile.seekg(0, ios::beg);

	string line;

	while (!uffFile.eof())
	{
		getline(uffFile, line);
		if (uffFile.eof())
			break;

		// trim white space. 
		size_t pos1 =  line.find_first_not_of(' ');
		line = line.substr(pos1 == string::npos ? 0 : pos1);
        // remove curriage return (CR) if txt file is created in MSDOS.
        pos1 = line.find('\r');
        if (pos1 != string::npos)
            line = line.substr(0, pos1);

        //std::cout << ".";
		// if Nodes information 
		if (line == "15")
		{
            //std::cout << "\n";
            //std::cout << "Nodes were found!\n";
			bExistNodes = true;
			posNodes = uffFile.tellg();
		}
		// Lines
		else if (line == "82")
		{
			//std::cout << "\n";
            //std::cout << "Lines were found!\n";
            bExistLines = true;
			posLines = uffFile.tellg();

		}
		// Surfaces
		else if (line == "2412")
		{
            //std::cout << "\n";
            //std::cout << "Surfaces were found!\n";
			bExistSurfaces = true;
			posSurface = uffFile.tellg();
		}
		// Data
		else if (line == "58")
		{
            //std::cout << "\n";
            //std::cout << "Data was found!\n";
			bExistData = true;
			m_vposData.push_back(uffFile.tellg());
		}
	}
    //std::cout << "\n\n";
}

const arma::mat& OiUniversalFormat::getNodes()
{
    return nodes_;
}

const arma::umat& OiUniversalFormat::getLines()
{
    return lines_;
}

const arma::umat& OiUniversalFormat::getSurfaces()
{
    return surfaces_;
}

const arma::mat& OiUniversalFormat::getData()
{
    return data_;
}

double OiUniversalFormat::getSamplingT()
{
    return samplingInterval_;
}

int OiUniversalFormat::getNumberOfSamples()
{
    if (vNumberOfSamples.empty())
        return 0;

    vector<int>::iterator it;
    it = std::min_element(vNumberOfSamples.begin(), vNumberOfSamples.end());
    if (it != vNumberOfSamples.end())
        return *it;
    else
        return 0;
}

bool OiUniversalFormat::existNodes()
{
    return bExistNodes;
}

bool OiUniversalFormat::existLines()
{
    return bExistLines;
}

bool OiUniversalFormat::existSurfaces()
{
    return bExistSurfaces;
}

bool OiUniversalFormat::existData()
{
    return bExistData;
}

} // namespace Oi

