// =====================================================================================
// 
//       Filename:  OiPersist.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-29 18:00:35
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


#ifndef  OIPERSIST_INC
#define  OIPERSIST_INC

#include	<string>

using std::string;

namespace Oi
{
    class PersistInterface
    {
        public:
            virtual ~PersistInterface(){}
        public:
            virtual void save() = 0;
            virtual void load() = 0;
    };

} // namespace Oi

#endif   // ----- #ifndef OIPERSIST_INC  -----
