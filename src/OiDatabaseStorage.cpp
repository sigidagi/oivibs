// =====================================================================================
//
// Filename: DatabaseStorage.cpp
//
// Description: DatabaseStorage
//
// Version: 1.0
// Created: 2010-06-15 14:02:38
// Revision: none
// Compiler: g++
//
// Author: Sigitas Dagilis (), sigidagi@gmail.com
// Company: sigidagis
//
// =====================================================================================

#include "OiDatabaseStorage.h"

#if defined(OI_USE_MYSQLPP)

#include "OiDatabaseTable.h"
#include "OiFileFormat.h"
#include "OiProcessing.h"
#include "OiUtil.h"

#include <iostream>
#include <stdexcept>

#define DATABASE "vibs_data" //
#define HOST "localhost" //
#define USER "root" //
#define PASSWORD "testas" //

namespace Oi
{
    DatabaseStorage::DatabaseStorage()
    {

    }

    /* *
* init function creates database with specified name and makes a connection
* which is stored in private variable mysqlpp::Connection connection_
*
* Second phase is to parse given file and store found data: nodes, lines, surfaces and recorded data
* into that database.
*
*/
    bool DatabaseStorage::init(const string& file)
    {
        if (file.empty())
        {
            std::cerr << "DatabaseStorage::init --\n";
            std::cerr << "File: is empty\n";
            return false;
        }
   
        string repository = Oi::stripToFileName(file);
        if (repository.empty())
        {
            std::cerr << "DatabaseStorage::init --\n";
            std::cerr << "Bad file name: " << file << "\n";
            return false;
        }
        
        mysqlpp::Connection con;

        // noexception is needed not to throw exception when such database exist.
        mysqlpp::NoExceptions ne(con);
        
        // try to create database and table
        if (!connectToDatabase(con))
        {
            std::cout << "Database: " << DATABASE << " do not exist. Creating new one!\n";
            if (!con.create_db(DATABASE))
            {
                std::cerr << "Error creating database: " << con.error() << std::endl;
                return false;
            }
            if(!createTable())
            {
                std::cerr << "Error creating table: Store.\n";
                return false;
            }
        }
 
        if (!existTable())
            createTable();
        
        return true;
    }

    bool DatabaseStorage::connectToDatabase(mysqlpp::Connection& con)
    {
        const char *server = 0;
        try
        {
            if (!con.connect(0, server, USER, PASSWORD))
                return false;
        }
        catch (std::exception& er)
        {
            std::cerr << "Attempt to connect to database - exception was thrown.\n";
            return false;
        }

        mysqlpp::NoExceptions ne(con);
        if (!con.select_db(DATABASE))
            return false;
        
        return true;
    }

    bool DatabaseStorage::existRepository(const string& name)
    {
        // NOT implemented jet!
        return false;
    }

    bool DatabaseStorage::existTable()
    {
        mysqlpp::Connection con;
        if (!connectToDatabase(con))
            return false;
    
        try
        {
            mysqlpp::Query query = con.query();
            // test if exist such tabe: if not - result would "false"
            query << "SELECT * FROM Store";

            mysqlpp::SimpleResult res = query.execute();
            if (!res)
                return false;
        }
        catch( std::exception& err)
        {
            std::cerr << "Query error " << err.what() << "\n";
            return false;
        }
       
        return true;
    }
    
    bool DatabaseStorage::createTable()
    {
        mysqlpp::Connection con;
        if (!connectToDatabase(con))
            return false;

        try
        {
            mysqlpp::Query query = con.query();
            std::cout << "Creating Store table... ";
            query <<
                "CREATE TABLE Store (" <<
                " file CHAR(30) NOT NULL, " <<
                " variable CHAR(30) NOT NULL, " <<
                " data BLOB NOT NULL, " <<
                " nrows INT NOT NULL, " <<
                " ncols INT NOT NULL, " <<
                " description MEDIUMTEXT NULL)";
            query.execute();
        }
        catch( std::exception& er)
        {
            std::cerr << "Query error: " << er.what() << std::endl;
            return false;
        }
        std::cout << "Succeeded!\n";
        return true;
    }
    
    void DatabaseStorage::write(std::stringstream& ss)
    {
        string repoName, variableName;
        ss >> repoName;

        if (!existTable())
            return;

        ss >> variableName;
        int nr, nc;
        ss >> nr >> nc;

        mysqlpp::Connection con;
        if (!connectToDatabase(con))
            return;

        try
        {
            mysqlpp::Query query = con.query();
            query << "INSERT INTO Store (file, variable, data, nrows, ncols) VALUES(\"" << repoName <<
            "\", \"" << variableName << "\", \"" << mysqlpp::escape << ss.str() << "\", " <<
            nr << ", " << nc << ")";

            mysqlpp::SimpleResult res = query.execute();

        }
        catch (const mysqlpp::BadQuery& er)
        {
            std::cerr << "Query error: " << er.what() << std::endl;
            return;
        }
        catch(const std::exception& er)
        {
            // Catch-all for any other MySQL++ exceptions
            std::cerr << "Error: " << er.what() << std::endl;
            return;
        }

    }

    void DatabaseStorage::read(std::stringstream& ss)
    {
        
        // NOT implemented
    }
    
 } // namespace Oi

#endif // OI_USE_MYSQLPP
