// =====================================================================================
// 
//       Filename:  DatabaseStorage.cpp
// 
//    Description:  DatabaseStorage
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

#include "OiDatabaseStorage.h"
#include "OiFileFormat.h"
#include "OiProcessing.h"
#include "OiUtil.h"

#include <iostream>
#include <stdexcept>

#define	DATABASE    "vibs_data"			//
#define	HOST        "localhost"	    //
#define	USER        "root"			//
#define	PASSWORD    "testas"        //

namespace Oi {

    DatabaseStorage::DatabaseStorage()
    {

    }

    mysqlpp::Connection& DatabaseStorage::getConnection()
    {
        return connection_;
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
   
        string tableName = Oi::stripToBaseName(file);
        if (tableName.empty())
        {
            std::cerr << "DatabaseStorage::init --\n";
            std::cerr << "Bad file name: " << file << "\n"; 
            return false;
        }

        // noexception is needed not to throw exception when such database exist.
        mysqlpp::NoExceptions ne(connection_);
        
        if (connectToDatabase())
        {
            std::cout << "Connection to database: " << DATABASE << " is established.\n";
        }
        // try to create database and table
        else  
        {
            std::cout << "Database: " << DATABASE << " do not exist. Creating new one!i\n";
            if (!connection_.create_db(DATABASE))
            {
                std::cerr << "Error creating database: " << connection_.error() << std::endl; 
                return false;
            }
            if(!createRepository(tableName))
            {
                std::cerr << "Error creating table: " <<  tableName << "\n";
                return false;
            }
        }
 
        if (!connection_.select_db(DATABASE))
        {
            std::cerr << "Error selecting database: " << connection_.error() << std::endl;
            return false;
        }
       
        // if connection to database is established - check if exist table with "baseName" name.

        if (!existRepository(tableName))
            createRepository(tableName);
        
        return true;
    }

    bool DatabaseStorage::connectToDatabase()
    {
        const char *server = 0;
        try 
        {
            if (!connection_.connect(0, server, USER, PASSWORD))
                return false;
        }
        catch (std::exception& er) 
        {
            std::cerr << "Attempt to connect to database - exception was thrown.\n";
            return false;
        }

        mysqlpp::NoExceptions ne(connection_);
        if (!connection_.select_db(DATABASE))
        {
            std::cerr << "Error selecting database: " << connection_.error() << std::endl;
            return false;
        }
        
        bConnected_ = true;
        return true;
    }

    bool DatabaseStorage::existRepository(const string& tableName)
    {
        mysqlpp::Query query = connection_.query();
        try 
        {
/*
 *            query << "SELECT count(*) FROM information_schema.tables " << 
 *                     "WHERE table_schema='" << DATABASE << "' " <<
 *                     "AND table_name='" << tableName << "'";
 *
 */
            query << "SHOW TABLES FROM " << DATABASE << "like " << tableName;           

            mysqlpp::SimpleResult res = query.execute();
            long numberOfRows = res.rows();
            if (numberOfRows == 0)
                return false;
        }
        catch( std::exception& err)
        {
            std::cerr << "Query error " << err.what() << "\n";
            return false;
        }
       
        return true;
    }
    
    bool DatabaseStorage::createRepository(const string& name)
    {
        mysqlpp::Query query = connection_.query();
        
        try
        {
            std::cout << "Creating table of " << name << std::endl;
            query << 
                "CREATE TABLE " << name << " (" <<
                " id INT UNSIGNED AUTO_INCREMENT, " << 
                " name CHAR(30) NOT NULL, " <<
                " data BLOB, " << 
                " nrows INT, " <<
                " ncols INT, " <<
                " description MEDIUMTEXT, " <<
                " PRIMARY KEY (id)" << 
                ")" <<
                "ENGINE = InnoDB " <<
                "CHARACTER SET utf8 COLLATE utf8_general_ci";

            query.execute();
        }
        catch( std::exception& er)
        {
            std::cerr << "Query error: " << er.what() << std::endl;
            return false;
        }
        
        tableName_ = name;
        std::cout << "Succeeded!\n\n";
        return true;
    }

/*
 *    // Function creates MySql tables according UFF file.
 *    bool DatabaseStorage::createTableOfData()
 *    {
 *        mysqlpp::Query query = connection_.query();
 *        
 *        try
 *        {
 *            std::cout << "Creating data table..." << std::endl;
 *            query.reset();
 *            query <<
 *                "CREATE TABLE measurement (" <<
 *        //		" id INT UNSIGNED AUTO_INCREMENT, " <<
 *                " label CHAR(30) NOT NULL, " <<
 *                " cdate CHAR(30), " <<			// creation date
 *                " ftype INT UNSIGNED, " <<		// function type must be 1 - time response
 *                " rnode INT UNSIGNED, " <<		// response node 
 *                " rdirection INT UNSIGNED, " << // response direction
 *                " dtype INT UNSIGNED, " <<		// data type must be 2 or 4 real single or double precision
 *                " dpoints INT UNSIGNED, " <<	// data points 
 *                " sinterval DOUBLE, " <<		// sampling interval
 *                " units CHAR(10), " <<			// measurement units
 *                " bdata BLOB " <<				// binary data
 *                ")";
 *            query.execute();
 *
 *        }
 *        catch (std::exception& er)
 *        {
 *            std::cerr << "Query error: " << er.what() << "std::endl";
 *            return false;
 *        }
 *        
 *        std::cout << "Succeeded!\n\n";
 *        return true;
 *    }
 */
    
    template<typename eT>
    void DatabaseStorage::saveMatrix(const arma::Mat<eT>& matrx, const string& name)
    {
        int size = matrx.n_rows;
        if (size == 0)
        {
            std::cerr << "Can NOT save: matrix is empty!\n";
            return;
        }
       
        if (!existRepository(tableName_))
            return;

        // Create blob from matrix 
        std::stringstream ss;

        // -- ens of blob --
        //
        mysqlpp::Query query = getConnection().query();
        
        try
        {
            query  << "INSERT INTO " << tableName_ << " (name, data, nrows. ncols) VALUES (\"" << 
                      name << "\", \"" << ss.str() << "\"," << 
                      (int)matrx.n_rows << "," << (int)matrx.n_cols << ")"; 

        }
        catch (std::exception& err)
        {
            std::cerr << "Query error: " << err.what() << std::endl;
            return;
        }
    }

/*
 *    void DatabaseStorage::saveLines(const arma::umat& lines)
 *    {
 *        if (lines.n_elem == 0 || lines.n_rows != 3)
 *            return;
 *
 *        createTableOfLines();
 *
 *        mysqlpp::Query query = getConnection().query();
 *        query  << "insert into %3:table values" <<
 *            "(%0, %1, %2)";
 *        query.parse();
 *
 *        query.template_defaults["table"] = "geolines";
 *
 *        for (size_t i = 0; i < lines.n_cols; ++i)
 *        {
 *            query.execute(lines(0,i), lines(1,i), lines(2,i));
 *        }
 *    }
 *
 *    void DatabaseStorage::saveSurfaces(const arma::umat& surfaces)
 *    {
 *        if (surfaces.n_elem == 0 || surfaces.n_rows != 4)
 *            return;
 *
 *        createTableOfSurfaces();
 *
 *        mysqlpp::Query query = getConnection().query();
 *        query  << "insert into %4:table values" <<
 *            "(%0, %1, %2, %3)";
 *        query.parse();
 *
 *        query.template_defaults["table"] = "geosurfaces";
 *
 *        for (size_t i = 0; i < surfaces.n_cols; ++i)
 *        {
 *            query.execute(surfaces(0,i), surfaces(1,i), surfaces(2,i), surfaces(3,i));
 *        }
 *    }
 */
    
/*
 *        // ------------------ Lines --------------------
 *        bool bRet = connect(tableName_);
 *        if (bRet == false)
 *            return ;
 *
 *        mysqlpp::Query query_nodes = connection_.query("select * from geonodes");
 *        mysqlpp::StoreQueryResult resNodes = query_nodes.store(); 
 *
 *        mysqlpp::Query query_lines = connection_.query("select * from geolines");
 *        mysqlpp::StoreQueryResult resLines = query_lines.store();
 *        
 *        
 *        if (!resLines || !resNodes)
 *            return;
 *
 *        size = (int)resLines.num_rows();
 *
 *        // allocation memeory
 *        //
 *        double** array = new double*[size];
 *        for (int n = 0; n < size; ++n)
 *            array[n] = new double[6];
 *
 *        int node1, node2;
 *        
 *        for (int idx = 0; idx < size; ++idx)
 *        {
 *            node1 = resLines[idx]["node1"];
 *            array[idx][0] = resNodes[node1-1]["x"];
 *            array[idx][1] = resNodes[node1-1]["y"];
 *            array[idx][2] = resNodes[node1-1]["z"];
 *            
 *            node2 = resLines[idx]["node2"];
 *            array[idx][3] = resNodes[node2-1]["x"];
 *            array[idx][4] = resNodes[node2-1]["y"];
 *            array[idx][5] = resNodes[node2-1]["z"];
 *        }
 *       
 *        //return array;
 *        
 *        // --------- end of Lines -----------------------
 *
 *        // ------------- Surfaces -----------------------
 *            bool bRet = connect(tableName_);
 *            if (bRet == false)
 *                return ;
 *
 *        
 *            mysqlpp::Query query = connection_.query("select * from surfaces");
 *            mysqlpp::StoreQueryResult resSurfaces = query.store();
 *            if (!resSurfaces)
 *                return ;
 *
 *            query.reset();
 *            query = connection_.query("select * from geonodes");
 *            mysqlpp::StoreQueryResult resNodes = query.store(); 
 *            if (!resNodes)
 *                return ;
 *
 *            
 *            size = (int)resSurfaces.num_rows();
 *
 *            // memory allocation
 *            double** array = new double*[size];
 *            for (int n = 0; n < size; ++n)
 *                array[n] = new double[9];
 *
 *            int node1, node2, node3;
 *           
 *           for (int idx = 0; idx < size; ++idx)
 *            {
 *                node1 = resSurfaces[idx]["node1"];
 *                array[idx][0] = resNodes[node1-1]["x"];
 *                array[idx][1] = resNodes[node1-1]["y"];
 *                array[idx][2] = resNodes[node1-1]["z"];
 *     
 *                node2 = resSurfaces[idx]["node2"];
 *                array[idx][3] = resNodes[node2-1]["x"];
 *                array[idx][4] = resNodes[node2-1]["y"];
 *                array[idx][5] = resNodes[node2-1]["z"];
 *                
 *                node3 = resSurfaces[idx]["node3"];
 *                array[idx][6] = resNodes[node3-1]["x"];
 *                array[idx][7] = resNodes[node3-1]["y"];
 *                array[idx][8] = resNodes[node3-1]["z"];
 *            }
 *            
 *            //return array;
 *  }
 */ 
    
    
} // namespace Oi
