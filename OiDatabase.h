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

using std::string;

class OiDatabase
{
    public:
	    bool init(const string& strFileName,  mysqlpp::Connection& con);
        bool connect(const string& dname, mysqlpp::Connection& conn);

        bool createTable_Nodes( mysqlpp::Connection& con);
        bool createTable_Lines( mysqlpp::Connection& con);
        bool createTable_Surfaces( mysqlpp::Connection& con);
        bool createTable_Data(mysqlpp::Connection& con);

        bool saveNodes();
};

#endif
