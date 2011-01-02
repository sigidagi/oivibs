// =====================================================================================
// 
//       Filename:  OiLocalStorage.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-13 12:02:48
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


#include "OiLocalStorage.h"

namespace Oi {
    
    // ---------  StorageInterface ---------------
    bool LocalStorage::init(const string& name)
    {
        // NOT implemented jet!
        return false;
    }

    bool LocalStorage::existRepository(const string& name)
    {
        // NOT implemented jet!
        return false;
    }
   
    template<typename T>
    void LocalStorage::write(SerializableObject<T>& object)
    {
                 
    }

    template<typename T>
    void LocalStorage::read(SerializableObject<T>& object)
    {

    }

    template<> void LocalStorage::write<arma::mat>(SerializableObject<arma::mat>& object);
    template<> void LocalStorage::write<arma::umat>(SerializableObject<arma::umat>& object);

} // namespace Oi
