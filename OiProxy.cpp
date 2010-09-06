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
#include "config.hpp"

Proxy::Proxy()
{
#if defined(OI_USE_MYSQLPP)
    impl_ = new OiDatabase;
#else
    impl_ = new OiGeometry;
#endif
}

Proxy::~Proxy()
{
    delete impl_;
}

void Proxy::getNodes(int* array, int& nnodes)
{
    impl->getNodes(array, nnodes);
}

void Proxy::getLines(double* array, int& nlines)
{
    impl->getLines(array, nlines);
}

void Proxy::getSurfaces(double* array, int& nsurfaces)
{
    impl->getSurfaces(array, nsurfaces);
}

