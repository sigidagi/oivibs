// =====================================================================================
// 
//       Filename:  OiDatabase.cpp
// 
//    Description:  Database
// 
//        Version:  1.0
//        Created:  2010-06-15 14:02:38
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Sigitas Dagilis (), sigidagi@gmail.com
//        Company:  sigidagis
// 
// =====================================================================================

#include "OiDatabase.h"
#include <iostream>
#include <stdexcept>

OiDatabase::OiDatabase(const string name) : bConnected_(false)
{
    if (name.empty())
        return;

    bConnected_ = connect(name);        
}

OiDatabase::OiDatabase() : bConnected_(false)
{

}


bool OiDatabase::isConnected()
{
    return bConnected_;
}

mysqlpp::Connection& OiDatabase::getConnection()
{
    return connection_;
}

bool OiDatabase::init(const string& strFileName)
{
    if (strFileName.empty())
        return false;

    // noexception is needed not to throw exception when such database exist.
    mysqlpp::NoExceptions ne(connection_);
    
    if (connect(strFileName))
    {
        std::cout << "Dropping existing sample data tables..." << std::endl;
        connection_.drop_db(strFileName);
    }
    
    if (!connection_.create_db(strFileName))
    {
        std::cerr << "Error creating DB: " << connection_.error() << std::endl; 
        return false;
    }

    if (!connection_.select_db(strFileName))
    {
        std::cerr << "Error selecting DB: " << connection_.error() << std::endl;
        return false;
    }
   
    bConnected_ = true;    
    return true;
}

bool OiDatabase::connect(const string& dname)
{
    if (dname.empty() )
        return false;

    const char *server = 0, *user = "root", *pass = "testas";

    try 
    {
        if (!connection_.connect(0, server, user, pass))
        {
            return false;
        }
    }
    catch (std::exception& er) 
    {
        return false;
    }

    mysqlpp::NoExceptions ne(connection_);
    if (!connection_.select_db(dname))
        return false;

    bConnected_ = true;
    return true;
}

bool OiDatabase::createTable_Lines()
{
    mysqlpp::Query query = connection_.query();

    try
    {
        std::cout << "Creating empty lines table..." << std::endl;
        query << 
            "CREATE TABLE geolines (" <<
            " id INT UNSIGNED AUTO_INCREMENT, " << 
            " node1 INT NOT NULL, " <<
            " node2 INT NOT NULL, " <<
            " PRIMARY KEY (id)" << 
            ")";

        query.execute();
    }
    catch( std::exception& er)
    {
        std::cerr << "Query error: " << er.what() << "std::endl";
        return false;
    }

    std::cout << "Succeeded!\n";
    std::cout << "\n";
    return true;
}

bool OiDatabase::createTable_Surfaces()
{
    mysqlpp::Query query = connection_.query();
    
    try
    {
        std::cout << "Creating empty surfaces table..." << std::endl;
        query << 
            "CREATE TABLE geosurfaces (" <<
            " id INT UNSIGNED AUTO_INCREMENT, "
            " node1 INT NOT NULL, " <<
            " node2 INT NOT NULL, " <<
            " node3 INT NOT NULL, " << 
            " PRIMARY KEY (id)" <<
            ")";
        query.execute();

    }
    catch (std::exception& er)
    {
        std::cerr << "Query error: " << er.what() << "std::endl";
        return false;
    }

    std::cout << "Succeeded!\n";
    std::cout << "\n";
    return true;
}

bool OiDatabase::createTable_Nodes()
{
    mysqlpp::Query query = connection_.query();

    // Creating tables;
    try
    {
        std::cout << "Creating Node table...\n";
        query << "CREATE TABLE geonodes (" <<
            " id INT NOT NULL, " <<
            " PRIMARY KEY(id), " <<
            " x DOUBLE, " << 
            " y DOUBLE, " <<
            " z DOUBLE " <<
            ")" <<
            "ENGINE = InnoDB " <<
            "CHARACTER SET utf8 COLLATE utf8_general_ci";

        query.execute();
    }
    catch (std::exception& er)
    {
        std::cerr << "Query error: " << er.what() << "std::endl";
        return false;
    }
    
    std::cout << "Succeeded!\n";
    std::cout << "\n";
    return true;
}

// Function creates MySql tables according UFF file.
bool OiDatabase::createTable_Data()
{
    mysqlpp::Query query = connection_.query();
    
    try
    {
        std::cout << "Creating data table..." << std::endl;
        query.reset();
        query <<
            "CREATE TABLE measurement (" <<
    //		" id INT UNSIGNED AUTO_INCREMENT, " <<
            " label CHAR(30) NOT NULL, " <<
            " cdate CHAR(30), " <<			// creation date
            " ftype INT UNSIGNED, " <<		// function type must be 1 - time response
            " rnode INT UNSIGNED, " <<		// response node 
            " rdirection INT UNSIGNED, " << // response direction
            " dtype INT UNSIGNED, " <<		// data type must be 2 or 4 real single or double precision
            " dpoints INT UNSIGNED, " <<	// data points 
            " sinterval DOUBLE, " <<		// sampling interval
            " units CHAR(10), " <<			// measurement units
            " bdata BLOB " <<				// binary data
            ")";
        query.execute();

    }
    catch (std::exception& er)
    {
        std::cerr << "Query error: " << er.what() << "std::endl";
        return false;
    }
    
    std::cout << "Succeeded!\n";
    std::cout << "\n";
    return true;
}

// Implementation of ProxyBase interface

void OiDatabase::getNodes(double** array, int& nnodes)
{
    if (!connect("test"))
        return;    

    mysqlpp::Query query = connection_.query("select * from geonodes");
    mysqlpp::StoreQueryResult res = query.store();
   
    nnodes = (int)res.num_rows(); 
    if (nnodes == 0)
        return;
    
    // allocation of memory of 2D array
    // Dealocation of memory should be carried out by CLIENT! 
    array = new double*[nnodes];
    for (int n = 0; n < nnodes; ++n)
        array[n] = new double[3];

    // assign values
	for (size_t i = 0; i < res.num_rows(); ++i)
    {
		array[i][0] = (res[i]["x"]);
		array[i][1] = (res[i]["y"]);
		array[i][2] = (res[i]["z"]);
    }
   
}

void OiDatabase::getLines(double** array, int& nlines)
{
    bool bRet = connect("test");
    if (bRet == false)
        return;

    mysqlpp::Query query_nodes = connection_.query("select * from geonodes");
    mysqlpp::StoreQueryResult resNodes = query_nodes.store(); 

    mysqlpp::Query query_lines = connection_.query("select * from geolines");
    mysqlpp::StoreQueryResult resLines = query_lines.store();
    
    
    if (!resLines || !resNodes)
        return;

    nlines = (int)resLines.num_rows();

    // allocation memeory
    //
    array = new double*[nlines];
    for (int n = 0; n < nlines; ++n)
        array[n] = new double[6];

    int node1, node2;
    
    for (int idx = 0; idx < nlines; ++idx)
    {
        node1 = resLines[idx]["node1"];
        array[idx][0] = resNodes[node1-1]["x"];
        array[idx][1] = resNodes[node1-1]["y"];
        array[idx][2] = resNodes[node1-1]["z"];
        
        node2 = resLines[idx]["node2"];
        array[idx][3] = resNodes[node2-1]["x"];
        array[idx][4] = resNodes[node2-1]["y"];
        array[idx][5] = resNodes[node2-1]["z"];
    }
    
}

void OiDatabase::getSurfaces(double** surfacearray, int& nsurfaces)
{
        bool bRet = connect("test");
        if (bRet == false)
            return;

    
        mysqlpp::Query query = connection_.query("select * from surfaces");
        mysqlpp::StoreQueryResult resSurfaces = query.store();
        if (!resSurfaces)
            return;

        query.reset();
        query = connection_.query("select * from geonodes");
        mysqlpp::StoreQueryResult resNodes = query.store(); 
        if (!resNodes)
            return;

        
        nsurfaces = (int)resSurfaces.num_rows();

        // memory allocation
        surfacearray = new double*[nsurfaces];
        for (int n = 0; n < nsurfaces; ++n)
            surfacearray[n] = new double[9];

        int node1, node2, node3;
       
       for (int idx = 0; idx < nsurfaces; ++idx)
        {
            node1 = resSurfaces[idx]["node1"];
            surfacearray[idx][0] = resNodes[node1-1]["x"];
            surfacearray[idx][1] = resNodes[node1-1]["y"];
            surfacearray[idx][2] = resNodes[node1-1]["z"];
 
            node2 = resSurfaces[idx]["node2"];
            surfacearray[idx][3] = resNodes[node2-1]["x"];
            surfacearray[idx][4] = resNodes[node2-1]["y"];
            surfacearray[idx][5] = resNodes[node2-1]["z"];
            
            node3 = resSurfaces[idx]["node3"];
            surfacearray[idx][6] = resNodes[node3-1]["x"];
            surfacearray[idx][7] = resNodes[node3-1]["y"];
            surfacearray[idx][8] = resNodes[node3-1]["z"];
        }
        
}

