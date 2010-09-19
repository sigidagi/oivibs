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

#include "OiProxy.h"
#include <string>
#include <memory>
#include <mysql++.h>
#include <armadillo>

using std::string;

namespace Oi {
    
    class DatabaseStorage : public ProxyInterface, public StorageInterface
    {
        private:
            DatabaseStorage();
            ~DatabaseStorage();
            DatabaseStorage(DatabaseStorage const&);
            DatabaseStorage& operator=(DatabaseStorage const&);

        public:
            mysqlpp::Connection& getConnection();
            static DatabaseStorage* Instance();

        // ProxyInterface 
        public:
            bool init(const string& file, int processName = 0);
            bool connect(const string& dataName);
            double** getNodes(int& size);
            double** getLines(int& size);
            double** getSurfaces(int& size);
            bool isConnected();

        // StorageInterface 
        public:
            void saveNodes(const arma::mat& nodes);
            void saveLines(const arma::umat& lines);
            void saveSurfaces(const arma::umat& surfaces);
            void saveData(const arma::mat& data);
            void saveSingularValues(const arma::mat& values);
            void saveSingularVectors(const arma::cx_mat& vectors);
           
        private:
            bool createTableOfNodes();
            bool createTableOfLines();
            bool createTableOfSurfaces();
            bool createTableOfData();

        private:
            static DatabaseStorage* instance_;
            string dbname_;
            bool bConnected_;
            mysqlpp::Connection connection_;
            
           
    };

} // namespace Oi

#endif
