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
//        Company:  sigidagis
// 
// =====================================================================================

#include "OiProxy.h"
#include "OiDatabaseStorage.h"
#include "OiLocalStorage.h"
#include "config.hpp"

namespace Oi {

    Proxy::Proxy() 
    {
        #if defined(OI_USE_MYSQLPP)
        impl_ = DatabaseStorage::Instance();
        #else
        impl_ = LocalStorage::Instance();
        #endif
    }

    Proxy::~Proxy()
    {

    }

    bool Proxy::init(const string& name, int processName)
    {
        return impl_->init(name, processName);
    }

    bool Proxy::connect(const string& name)
    {
        return impl_->connect(name);
    }

    double** Proxy::getNodes(int& size)
    {
        return impl_->getNodes(size);
    }

    double** Proxy::getLines(int& size)
    {
        return impl_->getLines(size);
    }

    double** Proxy::getSurfaces(int& size)
    {
        return impl_->getSurfaces(size);
    }

} // namespace Oi
