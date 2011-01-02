// =====================================================================================
// 
//       Filename:  OiLocalStorage.h
// 
//    Description:  Utilities functions
// 
//        Version:  1.0
//        Created:  2010-09-07 11:36:18
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


#ifndef _OIGEOMETRY_H
#define _OIGEOMETRY_H

#include <string>

#include "OiProxy.h"
#include "OiStorage.h"

using std::string;


namespace Oi {

    class LocalStorage : public StorageInterface
    {
        public:
            LocalStorage(){}
     
        // StorageInterface
        public: 
             bool init(const string& repoName);
             bool existRepository(const string& repoName); 
                
             template<typename T>   
             void write(SerializableObject<T>& object);
             template<typename T>
             void read(SerializableObject<T>& object);

    };


} // namespace Oi


#endif
