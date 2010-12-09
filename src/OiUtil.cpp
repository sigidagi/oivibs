// =====================================================================================
// 
//       Filename:  OiUtil.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-07 11:52:20
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

#include "OiUtil.h"

namespace Oi {

    void free2D(double** p2Darray, int length)
    {
        for (int i = 0; i < length; ++i)
            delete [] p2Darray[i];

        delete [] p2Darray;
        p2Darray = 0;
    }
    
    string stripToFileName(const string& pathToFile)
    {
        string fileName;
        string::size_type idx = pathToFile.rfind('/');
        if ( idx != string::npos)
        {
             fileName = pathToFile.substr(idx+1, string::npos);
        }
        else
            fileName = pathToFile;

        return fileName;
    }


    string stripToBaseName(const string& pathToFile)
    {
        string baseName;
        string::size_type idx = pathToFile.rfind('.');
    	if (idx != string::npos)
    	{
    		baseName = pathToFile.substr(0, idx);
    	}
    	else 
    		baseName = pathToFile;

        idx = baseName.rfind('/');
        if ( idx != string::npos)
        {
             baseName = baseName.substr(idx+1, string::npos);
        }
        
        return baseName;
    }
   
    string stripToExtension(const string& pathToFile)
    {

        string extensionName;
        string::size_type idx = pathToFile.rfind('.');
    	if (idx != string::npos)
    	{
            extensionName = pathToFile.substr(idx+1, string::npos);
    	}

        return extensionName;
    }

    string stripToPath(const string& pathToFile)
    {
        string path;
        string::size_type idx = pathToFile.rfind('/');
    	if (idx != string::npos)
    	{
    		path = pathToFile.substr(0, idx);
    	}
    	else 
    		path = ".";

        return path;
    }
} // namespace Oi
