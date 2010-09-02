#include "UniversalFormat.h"
#include "OiGeometry.h"
#include "OiDatabase.h"

#include <algorithm>
#include <sstream>
#include <unistd.h>

using namespace std;

UniversalFormat::UniversalFormat(const vector<string>& vFiles) : bExistNodes(false), bExistLines(false), bExistSurfaces(false), bExistData(false)
{
    
     if( !OiGeometry::Instance().getStatus().empty() )
     {
        OiGeometry::Instance().clearStatus();         
     }
    
    __vFiles.resize(vFiles.size());
    std::copy(vFiles.begin(), vFiles.end(), __vFiles.begin());
}

void UniversalFormat::parse()
{
	if (__vFiles.empty())
		return;

	for (size_t i = 0; i < __vFiles.size(); ++i)
	{
		string file = __vFiles[i];
       
/*
 *        size_t psize;
 *        char* path = NULL;
 *        path = getcwd(path, psize);
 *
 *        OiGeometry::Instance().__strStatus.push_back( string(path) );
 */
        string path = "/var/www/uploads";
        chdir(path.c_str());

        OiGeometry::Instance().__strStatus.push_back("File: " + file); 

    	string strBaseName;
        string strExtension;
    	string::size_type idx = file.find('.');
    	if (idx != string::npos)
    	{
    		strBaseName = file.substr(0, idx);
            strExtension = file.substr(idx+1, string::npos);
    	}
    	else 
    		strBaseName = file;

        idx = strBaseName.rfind('/');
        if ( idx != string::npos)
            strBaseName = strBaseName.substr(idx+1, string::npos);

        bool braw_data = false;
        if (strExtension == "txt")
        {
            bool status = m_matData.load(file, arma::raw_ascii);            
            if (status == false)
            {
                // save into status and return 
                continue;
            }
            bExistData = true;
            braw_data = true;
            continue;
        }

        //  Create database . Database  name is asociated with file name.
        mysqlpp::Connection con;
        OiDatabase database;
        bool bSuccess = database.init(strBaseName, con); 
        if (!bSuccess)
            return;

		uffFile.open(file.c_str(), ios::in);
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

            OiGeometry::Instance().__strStatus.push_back(" Found: ");
			if (bExistNodes)
            {
                OiGeometry::Instance().__strStatus.push_back("nodes ");
                database.createTable_Nodes(con);
				parseNodes(con);
            }
			if (bExistLines)
            {
                OiGeometry::Instance().__strStatus.push_back("lines ");
                database.createTable_Lines(con);
				parseLines(con);
            }
			if (bExistSurfaces)
            {
                OiGeometry::Instance().__strStatus.push_back("surfaces ");
                database.createTable_Surfaces(con);
				parseSurfaces(con);
            }
			if (bExistData)
			{
                OiGeometry::Instance().__strStatus.push_back("data ");
                
				for (size_t t = 0; t < m_vposData.size(); ++t)
					parseData(m_vposData[t], t);
                    
                m_matData.save(strBaseName + ".mat");     
			}
            if ( !bExistNodes && !bExistLines && !bExistSurfaces) 
            {
                OiGeometry::Instance().__strStatus.push_back("No geometry" );
            }
            if (!bExistData )
            {
                OiGeometry::Instance().__strStatus.push_back("No data" );
            }
            
            OiGeometry::Instance().__strStatus.push_back("\r" );
		}
        else
        {
            OiGeometry::Instance().__strStatus.push_back("Can not open file: " + file);
        }
       
        if (uffFile.is_open())
            uffFile.close();

	}

	// First file name is used to create database with the same name.
	string strBaseName;
	string::size_type idx = __vFiles[0].find('.');
	if (idx != string::npos)
	{
		strBaseName = __vFiles[0].substr(0, idx);
	}
	else 
		strBaseName = __vFiles[0];



	m_strFile = __vFiles[0];
}

UniversalFormat::~UniversalFormat()
{

}

void UniversalFormat::parseNodes(mysqlpp::Connection& con)
{
	if (!bExistNodes)
		return;

    mysqlpp::Query query = con.query();
    query  << "insert into %4:table values" <<
        "(%0, %1, %2, %3)";
    query.parse();

    query.template_defaults["table"] = "geonodes";

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
        
		OiGeometry::Instance().m_vNodes.insert( std::make_pair(nodeNumber, location) );

        query.execute(nodeNumber, x, y, z);
	}
    

//	bExistNodes = false;
}

void UniversalFormat::parseLines(mysqlpp::Connection& con)
{
	if (!bExistLines || OiGeometry::Instance().m_vNodes.empty() )
		return;

    mysqlpp::Query query = con.query();
    query  << "insert into %3:table values" <<
        "(%0, %1, %2)";
    query.parse();

    query.template_defaults["table"] = "geolines";

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
	OiGeometry::Instance().m_vLines.clear();

	for (size_t i = 0; i < vTempNodes.size(); i+=2)
	{
		Line sline;
		sline.node1 = vTempNodes[i];
		sline.node2 = vTempNodes[i+1];

        query.execute(i/2+1, vTempNodes[i], vTempNodes[i+1]);
		OiGeometry::Instance().m_vLines.push_back( sline );
	}
	
	//catch ( std::out_of_range outOfRange )
	//{
	//	std::cout << "\n\nExeption: " << outOfRange.what();
	//	return;
	//}

	//bExistLines = false;
}

void UniversalFormat::parseSurfaces(mysqlpp::Connection& con)
{
	if (!bExistSurfaces)
		return;
    
    mysqlpp::Query query = con.query();
    query  << "insert into %4:table values" <<
        "(%0, %1, %2, %3)";
    query.parse();

    query.template_defaults["table"] = "geosurfaces";

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

		Surface sSurface;
        int node1, node2, node3;
		ss >> node1 >>  node2 >> node3; 
        sSurface.node1 = node1;
        sSurface.node2 = node2;
        sSurface.node3 = node3;
        
		//catch ( std::out_of_range outOfRange)
		//{
		//	std::cout << "\n\nExeption: " << outOfRange.what();
		//	return;
		//}

        query.execute(SurfaceNumber, node1, node2, node3);
		OiGeometry::Instance().m_vSurfaces.push_back( sSurface );
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

    vSamplingInterval.push_back(dSamplingInterval);
    vNumberOfSamples.push_back(nNumberOfPoints);

	getline(uffFile, line);
	getline(uffFile, line);
	getline(uffFile, line);
	getline(uffFile, line);

    // first channel
    if (column == 0)
        // (rows, columns) - rows represent nummber of points in a channel, columns - 
        // number of channels.
        m_matData.set_size( nNumberOfPoints, (int)m_vposData.size() );
    else
    {
        // truncate matrix to a smaller number of columns.
        if ((int)m_matData.n_cols > nNumberOfPoints)
            m_matData.set_size( nNumberOfPoints, (int)m_vposData.size() );
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
            m_matData(nCount, column) = dValue;
            ++nCount;
        }
		while (ss >> dValue);
	
		ss.str("");
		ss.clear();
	}

}

const arma::Mat<double>& UniversalFormat::getData() const
{
    return m_matData;
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
        vSamplingInterval.push_back(dT);       

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


