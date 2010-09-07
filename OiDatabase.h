// =====================================================================================
// 
//       Filename:  OiDatabase.h
// 
//    Description:  Database 
// 
//        Version:  1.0
//        Created:  2010-06-15 13:56:00
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Sigitas Dagilis (), sigidagi@gmail.com
//        Company:  sigidagis
// 
// =====================================================================================

#ifndef _OIDATABASE_H
#define _OIDATABASE_H

#include <string>
#include <mysql++.h>
#include "OiProxy.h"

using std::string;

class OiDatabase : public ProxyBase
{
    public:
        OiDatabase(const string name);
        OiDatabase();

    public:
	    bool init(const string& strFileName);
        mysqlpp::Connection& getConnection();
        
        bool createTable_Nodes();
        bool createTable_Lines();
        bool createTable_Surfaces();
        bool createTable_Data();

        bool saveNodes();

    // ProxyBase interface
    public:
        void getNodes(double** array, int& nnodes);
        void getLines(double** array, int& nlines);
        void getSurfaces(double** array, int& nSurfaces);
        bool isConnected();
    
    private:
        bool connect(const string& dname);

    private:
        bool bConnected_;
        mysqlpp::Connection connection_;

};

#endif
