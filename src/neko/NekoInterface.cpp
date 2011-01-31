#include <iostream>
#include <neko.h>
#include <vector>

#include "../OiProxy.h"

using std::vector;

//using namespace Oi;
vector<string> convert2string(value varr)
{
    int sz = val_array_size(varr);
    vector<string> vFileNames;

    if (sz == 0)
        return vFileNames;

    
    for (int i = 0; i < sz; ++i)
    {
        if (val_is_string(val_array_ptr(varr)[i]))
            vFileNames.push_back( string(val_string(val_array_ptr(varr)[i])) );
    }

    return vFileNames;
}


value init( value varr )
{
    
    vector<string> fileList = convert2string(varr);
    if (fileList.empty())
        return val_false;
   
    vector<const char*> pcharList(fileList.size()+1);
    long i = fileList.size(); 
    pcharList[i] = 0;
    for(--i; i >= 0; --i) { pcharList[i] = fileList[i].c_str(); }

    
   
    // cration of object and initialization - parsing data. 
    // search for goemetry: nodes, lines and surfaces and search for data.
    Oi::Proxy proxy;
    
    if ( !proxy.init((int)pcharList.size(), &pcharList[0] ))
        return val_false;

    return val_true;
}

value getPSD()
{
    return val_null;
}

value getNodes( value varr )
{
      vector<string> fileList = convert2string(varr); 
      if (fileList.empty())
          return alloc_array(0);

      Oi::Proxy proxy;

      for (size_t i = 0; i < fileList.size(); ++i)
      {
          if (!proxy.connect(fileList[i]))
              continue;
          
          int nrows(0), ncols(0);
          const double* nodesarray = proxy.getNodes(nrows, ncols);
          if (nodesarray == 0 || ncols != 3)
              continue;

          value arr = alloc_array(nrows*ncols);
          for (int i = 0; i < nrows; ++i)
          {
                value o1  = alloc_object(NULL);

                alloc_field(o1, val_id("x"), alloc_float(nodesarray[0*nrows+i]));
                alloc_field(o1, val_id("y"), alloc_float(nodesarray[1*nrows+i]));
                alloc_field(o1, val_id("z"), alloc_float(nodesarray[2*nrows+i]));
         
                val_array_ptr(arr)[i] = alloc_object(o1);
          }
        
          return arr;
             
      }
    
      return alloc_array(0);
}

static value getNumberOfNodes()
{
    return val_null;
}

static value getNodeLocation(value iNode)
{
	value o = alloc_object(NULL);

	if (!val_is_int(iNode))
		return o;

	return o;
}

static value getLines(value varr)
{

    vector<string> fileList = convert2string(varr);
    if (fileList.empty())
        return alloc_array(0);

    
    Oi::Proxy proxy;
    for (size_t i = 0; i < fileList.size(); ++i)
    {
        if (!proxy.connect(fileList[i]))
            continue;

        int nrowsLines(0), ncolsLines(0);
        const unsigned int* linesarray = proxy.getLines(nrowsLines, ncolsLines);
        if (linesarray == 0 || ncolsLines != 2)
            continue;
    
        int nrows(0), ncols(0);
        const double* nodesarray = proxy.getNodes(nrows, ncols);
        if (nodesarray == 0 || ncols != 2)
            continue;

        value arr = alloc_array(ncolsLines*nrowsLines);
        int node1, node2;
        value o1,o2;
        for (int idx = 0; idx < nrowsLines; ++idx)
        {
            o1  = alloc_object(NULL);
            node1 = linesarray[idx+0*nrowsLines] - 1;
            alloc_field(o1, val_id("x"), alloc_float(nodesarray[0*nrows+node1]));
            alloc_field(o1, val_id("y"), alloc_float(nodesarray[1*nrows+node1]));
            alloc_field(o1, val_id("z"), alloc_float(nodesarray[2*nrows+node1]));
            
            o2 = alloc_object(NULL);
            node2 = linesarray[idx+1*nrowsLines] - 1;
            alloc_field(o2, val_id("x"), alloc_float(nodesarray[node2+0*nrows]));
            alloc_field(o2, val_id("y"), alloc_float(nodesarray[node2+1*nrows]));
            alloc_field(o2, val_id("z"), alloc_float(nodesarray[node2+2*nrows]));

            val_array_ptr(arr)[2*idx] = alloc_object(o1);
            val_array_ptr(arr)[2*idx+1] = alloc_object(o2);
        }
        
        return arr;

    }
    
    return alloc_array(0);
}


value getSurfaces( value varr )
{
    vector<string> fileNames = convert2string( varr );
    if (fileNames.empty())
        return alloc_array(0);


    Oi::Proxy proxy;
    for (size_t i = 0; i < fileNames.size(); ++i)
    {
        if (!proxy.connect(fileNames[i]))
            continue;

        int nrowsSurf(0), ncolsSurf(0);
        const unsigned int* surfacearray = proxy.getSurfaces(nrowsSurf, ncolsSurf);
        if ( surfacearray == 0 || ncolsSurf != 3)
            continue;
       
        int nrows(0), ncols(0);
        const double* nodesarray = proxy.getNodes(nrows, ncols); 
        if (nodesarray == 0 || ncols != 3)
            continue;
        
        int node1, node2, node3;
        value o1, o2, o3;
        value arr = alloc_array(nrowsSurf*ncolsSurf);
        
        for (int idx = 0; idx < nrowsSurf; ++idx)
        {
            o1 = alloc_object(NULL);
            node1 = surfacearray[idx+0*nrowsSurf] -1;
            alloc_field(o1, val_id("x"), alloc_float(nodesarray[node1+0*nrows]));
            alloc_field(o1, val_id("y"), alloc_float(nodesarray[node1+1*nrows]));
            alloc_field(o1, val_id("z"), alloc_float(nodesarray[node1+2*nrows]));

            o2 = alloc_object(NULL);
            node2 = surfacearray[idx+1*nrowsSurf] -1;
            alloc_field(o2, val_id("x"), alloc_float(nodesarray[node2+0*nrows]));
            alloc_field(o2, val_id("y"), alloc_float(nodesarray[node2+1*nrows]));
            alloc_field(o2, val_id("z"), alloc_float(nodesarray[node2+2*nrows]));

            o3 = alloc_object(NULL);
            node3 = surfacearray[idx+2*nrowsSurf] -1;
            alloc_field(o3, val_id("x"), alloc_float(nodesarray[node3+0*nrows]));
            alloc_field(o3, val_id("y"), alloc_float(nodesarray[node3+1*nrows]));
            alloc_field(o3, val_id("z"), alloc_float(nodesarray[node3*2*nrows]));


            val_array_ptr(arr)[3*idx] = alloc_object(o1);
            val_array_ptr(arr)[3*idx+1] = alloc_object(o2);
            val_array_ptr(arr)[3*idx+2] = alloc_object(o3);
        }
       
        return arr;

    }
    
    return alloc_array(0);
}

static value getNumberOfLines()
{
    return val_null;
}

static value getStatus()
{
/*
 *    vector<string> vstr = OiGeometry::Instance().getStatus();
 *    if (vstr.empty())
 *    {
 *        value arr = alloc_array(1);
 *        val_array_ptr(arr)[0] = alloc_string("Status is empty");
 *
 *        return arr;   
 *    }
 *   
 *    value arr = alloc_array( (int)vstr.size() );
 *    for (size_t t = 0; t < vstr.size(); ++t)
 *    {
 *        string str = vstr[t];
 *        val_array_ptr(arr)[t] = alloc_string( str.c_str() );
 *    }
 */
    
    value arr = val_null;

    return arr;

     /*  
     *char* buf = new char[strlen(str.c_str())];
     *strcpy(buf, str.c_str());
     *  
     */
    
/*
 *    int lenstr = strlen(str.c_str());
 *
 *    char* buf = (char*)malloc(lenstr + 1);
 *    memcpy(buf, str.c_str(), lenstr);
 *    buf[lenstr] = 0;
 *
 *    value ret = alloc_string(buf);
 *    free(buf);
 *
 *    return ret;
 */
    
}

static value say()
{
    string str = "hello from neko too!\n";
    /*
     *char* buf = new char[strlen(str.c_str())];
     *strcpy(buf, str.c_str());
     *
	 *value valstr =  alloc_string(buf);
     *delete []buf;
     */

    return alloc_string(str.c_str());
}

DEFINE_PRIM(getNodes, 1);
DEFINE_PRIM(getNumberOfNodes,0);
DEFINE_PRIM(getNumberOfLines, 0);
DEFINE_PRIM(getLines, 1);
DEFINE_PRIM(getSurfaces, 1);
DEFINE_PRIM(getPSD, 0);

DEFINE_PRIM(getNodeLocation, 1);
DEFINE_PRIM(getStatus, 0);
DEFINE_PRIM(say, 0);
DEFINE_PRIM(init, 1);

//DEFINE_PRIM_MULT(init);
