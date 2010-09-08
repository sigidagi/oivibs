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

#include "UniversalFormat.h"
#include "OiProxy.h"
#include <string>
#include <mysql++.h>

using std::string;

namespace Oi {

class OiDatabase : public ProxyBase
{
    public:
        OiDatabase(const string name = "");

    public:
        mysqlpp::Connection& getConnection();
        
       bool saveNodes();

    // ProxyBase interface
    public:
	    bool init(const string strFileName);
        void getNodes(double** array, int& nnodes);
        void getLines(double** array, int& nlines);
        void getSurfaces(double** array, int& nSurfaces);
        bool isConnected();
    
    private:
        bool connect(const string& dname);
        bool createTable_Nodes();
        bool createTable_Lines();
        bool createTable_Surfaces();
        bool createTable_Data();

        void saveNodes(const arma::mat& nodes);
        void saveLines(const arma::umat& lines);
        void saveSurfaces(const arma::umat& surfaces);

    private:
        
        string name_;
        bool bConnected_;
        mysqlpp::Connection connection_;
        UniversalFormat uff_;

};

} // namespace Oi

#endif
