// =====================================================================================
// 
//       Filename:  OiDatabaseTable.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-10-06 10:18:02
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Sigitas Dagilis (), sigidagi@gmail.com
//        Company:  sigidagis
// 
// =====================================================================================

#ifndef  OIDATABASETABLE_INC
#define  OIDATABASETABLE_INC

#include    "config.hpp"

#if defined(OI_USE_MYSQLPP)

#include	<mysql++.h>
#include	<ssqls.h>

// The following is calling a very complex macro which will create
// "struct store", which has the member variables:
//
//   sql_char file;
//   ...
//   Null<sql_mediumtext> description;
//
// plus methods to help populate the class from a MySQL row.  See the
// SSQLS sections in the user manual for further details.

namespace Oi
{
    sql_create_6(Store, 1, 6,
       mysqlpp::sql_char, file,
       mysqlpp::sql_char, variable,
       mysqlpp::sql_blob, data,
       mysqlpp::sql_int, nrows,
       mysqlpp::sql_int, ncols,
       mysqlpp::Null<mysqlpp::sql_mediumtext>, description) 

} // namespace Oi


#endif   // OI_USE_MYSQLPP
#endif   // ----- #ifndef OIDATABASETABLE_INC  -----

