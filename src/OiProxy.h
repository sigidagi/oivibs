// =====================================================================================
// 
//       Filename:  OiProxy.h
// 
//    Description:  Proxy class for object decoupling
// 
//        Version:  1.0
//        Created:  2010-09-06 16:26:16
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

#ifndef _OIPROXY_H
#define _OIPROXY_H

#include "OiStorage.h"
#include <string>

using std::string;

#define DLL __atribute__(dllexport)

/*
 *#ifdef __cplusplus
 *extern "C"
 *{
 *#endif
 *
 *namespace Oi
 *{
 *
 *} // namespace Oi
 *
 *#ifdef __cplusplus
 *}
 *#endif
 */

namespace Oi {

    class ProxyInterface
    {
        public:
            virtual bool init(int argc, const char** fileList, int processName = 0) = 0;
            virtual bool connect(const string& name) = 0;
            virtual double** getNodes(int& ) = 0;
            virtual unsigned int** getLines(int& ) = 0;
            virtual unsigned int** getSurfaces(int& ) = 0;
    };
} // namespace Oi


namespace Oi {

    class Proxy : public ProxyInterface
    {
        public:
            Proxy(); 
            ~Proxy ();

        // Interfaces
        public:
            bool init(int argc, const char** fileList, int processName = 0);
            bool connect(const string& name);
            double** getNodes(int& size);
            unsigned int** getLines(int& size);
            unsigned int** getSurfaces(int& size);

        private:
            ProxyInterface* impl_; 
    };

} // namespace Oi

#endif
