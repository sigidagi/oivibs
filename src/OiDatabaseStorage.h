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

#if (OI_USE_MYSQLPP)

#ifndef _OIDATABASE_H
#define _OIDATABASE_H

#include    "OiProxy.h"
#include    <string>
#include    <memory>
#include    <mysql++.h>
#include    <armadillo>
#include    <sstream>
#include	<armadillo>

using std::string;

namespace Oi {
 
    class DatabaseStorage : public StorageInterface
    {
        public:
            DatabaseStorage();

        private:
            bool createTable();
            bool existTable();
            bool connectToDatabase(mysqlpp::Connection& con);

 
        // StorageInterface 
        public:
            bool init(const string& repoName);
            bool existRepository(const string& name);
             
            void write(std::stringstream& ss);
            void read(std::stringstream& ss); 
     };
     

} // namespace Oi

#endif
#endif // OI_USE_MYSQLPP

