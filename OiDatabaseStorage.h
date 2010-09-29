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

#include    "OiProxy.h"
#include    <string>
#include    <memory>
#include    <mysql++.h>
#include    <armadillo>

using std::string;

namespace Oi {
    
    class DatabaseStorage : public StorageInterface
    {
        public:
            DatabaseStorage();

            mysqlpp::Connection& getConnection();
            template<typename eT>
            void saveMatrix(const arma::Mat<eT>& data, const string& name);

        private:
            bool init(const string& name);
            bool createTable(const string& name);
            bool existTable(const string& name);
            bool connectToDatabase();

            string tableName_;
            bool bConnected_;
            mysqlpp::Connection connection_;
 
        // StorageInterface 
        public:
        //    bool init(const string& repoName);

            template<class T>
            void write(const string& repoName, const string& name, const T& variable)
            {
                if (!connectToDatabase() || !existTable(repoName))
                    return;

                                

            }
            template<class T>
            void read(const string& repoName, const string& name, T& variable)
            {

            }
           
           
    };

} // namespace Oi

#endif
