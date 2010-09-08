#include "UniversalFormat.h"
#include "OiGeometry.h"
#include "OiDatabase.h"
#include "OiUtil.h"

#include <algorithm>
#include <sstream>
#include <unistd.h>

using namespace std;

namespace Oi {

UniversalFormat::UniversalFormat() : bExistNodes(false), bExistLines(false), bExistSurfaces(false), bExistData(false)
{
    
}

void UniversalFormat::parse(const string file)
{
	if (file.empty())
		return;

    string strExtension = Oi::stripToExtension(file);

    bool braw_data = false;
    if (strExtension == "txt")
    {
        bool status = m_Data.getData().load(file, arma::raw_ascii);            
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

UniversalFormat::~UniversalFormat()
{

}

void UniversalFormat::parseNodes()
{
	if (!bExistNodes)
		return;

    nodes_.reset();
	uffFile.clear();
	uffFile.seekg(posNodes, ios::beg);

	int nodeNumber, temp; 
	string line;
	stringstream ss;

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
        double x, y, z;     

		Point3D location;
		ss >> x >> y >> z;
        location.x = x;
        location.y = y;
        location.z = z;
       
        nodes_ << nodeNumber << x << y << z << arma::endr;
	}
    
}

void UniversalFormat::parseLines()
{
	if (!bExistLines /* || OiGeometry::Instance().m_vNodes.empty()*/ )
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

	vector<int>::iterator lastPos = std::remove(vTempNodes.begin(), vTempNodes.end(), 0);
	if (lastPos == vTempNodes.end())
		return;

	vTempNodes.erase(lastPos, vTempNodes.end());

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

void UniversalFormat::parseSurfaces()
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
		
		if ( FEDescriptor!=91 ) // Only thin shell triangular elements allowed
			return;

		if ( NumberOfElementNodes!=3 ) // There need to be three nodes
			return;

		getline(uffFile, line);
		ss.str("");
		ss.clear();
		ss << line;

        int node1, node2, node3;
		ss >> node1 >>  node2 >> node3; 
        
		//catch ( std::out_of_range outOfRange)
		//{
		//	std::cout << "\n\nExeption: " << outOfRange.what();
		//	return;
		//}
    
        surfaces_ << node1 << node2 << node3 << arma::endr; 
        
	}
	//bExistSurfaces = false;
}

void UniversalFormat::parseData(const int pos, int column )
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

    m_Data.vSamplingInterval.push_back(dSamplingInterval);
    m_Data.vNumberOfSamples.push_back(nNumberOfPoints);

	getline(uffFile, line);
	getline(uffFile, line);
	getline(uffFile, line);
	getline(uffFile, line);

    // first channel
    if (column == 0)
        // (rows, columns) - rows represent nummber of points in a channel, columns - 
        // number of channels.
        m_Data.getData().set_size( nNumberOfPoints, (int)m_vposData.size() );
    else
    {
        // truncate matrix to a smaller number of columns.
        if ((int)m_Data.getData().n_cols > nNumberOfPoints)
            m_Data.getData().set_size( nNumberOfPoints, (int)m_vposData.size() );
    }

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
            m_Data.getData()(nCount, column) = dValue;
            ++nCount;
        }
		while (ss >> dValue);
	
		ss.str("");
		ss.clear();
	}

}

void UniversalFormat::searchForSamplingT()
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
        m_Data.vSamplingInterval.push_back(dT);       

}

void UniversalFormat::searchForData()
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

const arma::mat& UniversalFormat::getNodes()
{
    return nodes_;
}

const arma::umat& UniversalFormat::getLines()
{
    return lines_;
}

const arma::umat& UniversalFormat::getSurfaces()
{
    return surfaces_;
}

bool UniversalFormat::existNodes()
{
    return bExistNodes;
}

bool UniversalFormat::existLines()
{
    return bExistLines;
}

bool UniversalFormat::existSurfaces()
{
    return bExistSurfaces;
}

bool UniversalFormat::existData()
{
    return bExistData;
}

} // namespace Oi

