#include <iostream>
#include <vector>

#include "OiProxy.h"

using std::vector;

template<typename T>
void free2D(T** p2Darray, int length)
{
    for (int i = 0; i < length; ++i)
        delete [] p2Darray[i];

    delete [] p2Darray;
    p2Darray = 0;
}


extern "C" {

void* create()
{
    

    // cration of object and initialization - parsing data. 
    // search for goemetry: nodes, lines and surfaces and search for data.
   return new Oi::Proxy; 

/*
 *    for (size_t i = 0; i < fileList.size(); ++i)
 *    {
 *        if ( !proxy.init(fileList[i]) )
 *            return false;
 *    }
 *
 *    return true;
 */
}

} // extern C

