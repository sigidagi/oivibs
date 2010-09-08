// =====================================================================================
// 
//       Filename:  OiUtil.h
// 
//    Description:  Utilities functions
// 
//        Version:  1.0
//        Created:  2010-09-07 11:36:18
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Sigitas Dagilis (), sigidagi@gmail.com
//        Company:  sigidagis
// 
// =====================================================================================

#ifndef _OIUTIL_H
#define _OIUTIL_H

#include <string>

using std::string;

namespace Oi
{
    void free2D(double**, int);
    
    string stripToBaseName(const string& pathToFile);
    string stripToFileName(const string& pathToFile);
    string stripToExtension(const string& pathToFile);
    string stripToPath(const string& pathToFile);
}

#endif
