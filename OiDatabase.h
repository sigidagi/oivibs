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

#include "OiProcessing.h"
#include "OiProxy.h"
#include <string>
#include <memory>
#include <mysql++.h>

using std::string;
class OiFormat;

namespace Oi {

class OiDatabase : public ProxyBase
{
    private:
        OiDatabase(){};
        OiDatabase(OiDatabase const&){};
        OiDatabase& operator=(OiDatabase const&){ return *this;};

    public:
        mysqlpp::Connection& getConnection();
       
        
        static OiDatabase* Instance();
        bool saveNodes();

    // ProxyBase interface
    public:
	    bool init(const string strFileName);
        bool start();
        void getNodes(double** array, int& nnodes);
        void getLines(double** array, int& nlines);
        void getSurfaces(double** array, int& nSurfaces);
        bool isConnected();
    
    private:
        bool connect(const string dname);
        bool createTable_Nodes();
        bool createTable_Lines();
        bool createTable_Surfaces();
        bool createTable_Data();

        void saveNodes(const arma::mat& nodes);
        void saveLines(const arma::umat& lines);
        void saveSurfaces(const arma::umat& surfaces);

    private:
        
        static OiDatabase* instance_;
        string name_;
        bool bConnected_;
        mysqlpp::Connection connection_;
        
        std::auto_ptr<OiFormat> pFormat_;
        OiProcessing proc_;
        
};

} // namespace Oi

#endif
