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


bool OiDatabase::init(const string& strFileName, mysqlpp::Connection& con)
{
    if (strFileName.empty())
        return false;

    // noexception is needed not to throw exception when such database exist.
    mysqlpp::NoExceptions ne(con);
    if (connect(strFileName, con))
    {
        std::cout << "Dropping existing sample data tables..." << std::endl;
        con.drop_db(strFileName);
    }
    
    if (!con.create_db(strFileName))
    {
        std::cerr << "Error creating DB: " << con.error() << std::endl; 
        return false;
    }

    if (!con.select_db(strFileName))
    {
        std::cerr << "Error selecting DB: " << con.error() << std::endl;
        return false;
    }
    
    return true;
}

bool OiDatabase::connect(const string& dname, mysqlpp::Connection& con)
{
    if (dname.empty() )
        return false;

    const char *server = 0, *user = "root", *pass = "test";

    try 
    {
//        std::cout << "Connecting to database server...\n";
        if (!con.connect(0, server, user, pass))
        {
//            std::cerr << "can't connect to database server\n";
            return false;
        }
//        std::cout << "Succeded!\n";
    }
    catch (std::exception& er) 
    {
//        std::cerr << "Connection failed: " << er.what() << std::endl;
        return false;
    }

    mysqlpp::NoExceptions ne(con);
    if (!con.select_db(dname))
        return false;

    return true;
}

bool OiDatabase::createTable_Lines(mysqlpp::Connection& con)
{
    mysqlpp::Query query = con.query();

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

bool OiDatabase::createTable_Surfaces(mysqlpp::Connection& con)
{
    mysqlpp::Query query = con.query();
    
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

bool OiDatabase::createTable_Nodes(mysqlpp::Connection& con)
{
    mysqlpp::Query query = con.query();

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
bool OiDatabase::createTable_Data(mysqlpp::Connection& con)
{
    mysqlpp::Query query = con.query();
    
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

