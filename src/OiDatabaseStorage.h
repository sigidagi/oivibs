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
//  
//     This file is part of the OiVibrations C++ library.
//     It is provided without any warranty of fitness
//     for any purpose. You can redistribute this file
//     and/or modify it under the terms of the GNU
//     General Public License (GPL) as published
//     by the Free Software Foundation, either version 3
//     of the License or (at your option) any later version.
//     (see http://www.opensource.org/licenses for more info)
// 
// =====================================================================================


#ifndef _OIDATABASE_H
#define _OIDATABASE_H

#include    "config.hpp"

#if defined(OI_USE_MYSQLPP)

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

#endif // OI_USE_MYSQLPP
#endif

