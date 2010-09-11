// =====================================================================================
// 
//       Filename:  DatabaseStorage.h
// 
//    Description:  DatabaseStorage 
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
class FileFormatInterface;

namespace Oi {

    class DatabaseStorage : public ProxyInterface
    {
        private:
            DatabaseStorage(){};
            DatabaseStorage(DatabaseStorage const&){};
            DatabaseStorage& operator=(DatabaseStorage const&){ return *this;};

        public:
            mysqlpp::Connection& getConnection();
            static DatabaseStorage* Instance();

        // ProxyInterface interface
        public:
            bool init(const string& file);
            bool connect(const string& dataName);
            void getNodes(double** array, int& size);
            void getLines(double** array, int& size);
            void getSurfaces(double** array, int& size);
            bool isConnected();
        
        private:
            bool createTable_Nodes();
            bool createTable_Lines();
            bool createTable_Surfaces();
            bool createTable_Data();

            void saveNodes(const arma::mat& nodes);
            void saveLines(const arma::umat& lines);
            void saveSurfaces(const arma::umat& surfaces);
            void saveData(const arma::mat& data);

        private:
            
            static DatabaseStorage* instance_;
            string name_;
            bool bConnected_;
            mysqlpp::Connection connection_;
            
            FileFormatInterface* fileFormat_;
            Processing proc_;
            
    };

} // namespace Oi

#endif
