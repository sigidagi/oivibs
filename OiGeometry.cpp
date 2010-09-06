#include "config.hpp"
#include "OiGeometry.h"


//OiGeometry* OiGeometry::instance_ = 0;

OiGeometry& OiGeometry::Instance()
{
    static OiGeometry thisOiGeometry;
    return thisOiGeometry;
}


/*
 *OiGeometry* OiGeometry::Instance()
 *{
 *    if (!instance_)
 *        instance_ = new OiGeometry;
 *    
 *    return instance_;
 *}
 */

void OiGeometry::getNodes(int* array, int& nnodes)
{
 
}

void OiGeometry::getLines(double* array, int& nlines)
{

}

void OiGeometry::getSurfaces(double* array, int& nsurfaces)
{

}


vector<string> OiGeometry::getStatus()
{
    return __strStatus;
}

void OiGeometry::clearStatus()
{
    __strStatus.clear();
}

