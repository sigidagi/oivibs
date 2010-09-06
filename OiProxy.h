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
//        Company:  sigidagis
// 
// =====================================================================================

#ifndef _OIPROXY_H
#define _OIPROXY_H

class ProxyBase
{
    public:
        virtual ~ProxyBase(){}
    
    // Interfaces
    public:
        virtual void getNodes(int*, int& ) = 0;
        virtual void getLines(double*, int& ) = 0;
        virtual void getSurfaces(double*, int& ) = 0;
};

class Proxy : ProxyBase {
    public:
        Proxy(); 
        ~Proxy ();

    // Interfaces
    public:
        void getNodes(int*, int& nnnodes);
        void getLines(double*, int& nlines);
        void getSurfaces(double*, int& nSurfaces);

    private:
        ProxyBase* impl_; 
};

#endif
