// =====================================================================================
// 
//       Filename:  OiProxy.cpp
// 
//    Description:  Implementation of Object decoupling
// 
//        Version:  1.0
//        Created:  2010-09-06 20:12:12
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

#include "OiProxy.h"
#include "OiRoot.h"
#include "config.hpp"

namespace Oi {

    Proxy::Proxy() 
    {
        impl_ = Root::Instance();
    }

    Proxy::~Proxy()
    {

    }

    bool Proxy::init(int argc, const char** fileList, int processName)
    {
        return impl_->init(argc, fileList, processName);
    }

    bool Proxy::connect(const string& name)
    {
        return impl_->connect(name);
    }

    const double* Proxy::getNodes(int& nrows, int& ncols) const
    {
        return impl_->getNodes(nrows, ncols);
    }

    const unsigned int* Proxy::getLines(int& nrows, int& ncols) const
    {
        return impl_->getLines(nrows, ncols);
    }

    const unsigned int* Proxy::getSurfaces(int& nrows, int& ncols) const
    {
        return impl_->getSurfaces(nrows, ncols);
    }

    const double* Proxy::getSingularValues(const string& fileName, int& nrows, int& ncols) const 
    {
        return impl_->getSingularValues(fileName, nrows, ncols);
    }
} // namespace Oi
