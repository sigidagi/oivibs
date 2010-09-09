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
#include "OiDatabase.h"
#include "OiGeometry.h"
#include "config.hpp"

namespace Oi {

    Proxy::Proxy() 
    {
        #if defined(OI_USE_MYSQLPP)
        impl_ = OiDatabase::Instance();
        #else
        impl_ = OiGeometry::Instance();
        #endif
    }

    Proxy::~Proxy()
    {

    }

    bool Proxy::init(const string name)
    {
        return impl_->init(name);
    }

    bool Proxy::connect(const string name)
    {
        return impl_->connect(name);
    }

    bool Proxy::start()
    {
        return impl_->start();
    }

    void Proxy::getNodes(double** array, int& nnodes)
    {
        impl_->getNodes(array, nnodes);
    }

    void Proxy::getLines(double** array, int& nlines)
    {
        impl_->getLines(array, nlines);
    }

    void Proxy::getSurfaces(double** array, int& nsurfaces)
    {
        impl_->getSurfaces(array, nsurfaces);
    }

} // namespace Oi
