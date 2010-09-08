#include <iostream>
#include <neko.h>
#include <vector>

#include "OiProxy.h"
#include "OiUtil.h"

using std::vector;
//using namespace Oi;

value init( value varr )
{
    int sz = val_array_size(varr);

    if (sz == 0)
        return val_false;

    vector<string> vFileNames;
    int i = 0;

    for (i = 0; i < sz; ++i)
    {
        if (val_is_string(val_array_ptr(varr)[i]))
            vFileNames.push_back( string(val_string(val_array_ptr(varr)[i])) );
    }

    if (vFileNames.empty())
        return val_false;
    
    // cration of object and initialization - parsing data. 
    // search for goemetry: nodes, lines and surfaces and search for data.
    Oi::Proxy proxy;
    for (i = 0; i < (int)vFileNames.size(); ++i)
    {
        if ( !proxy.init(vFileNames[i]) )
            return val_false;
    }

    return val_true;
}

value getPSD()
{
    return val_null;
}

value getNodes()
{
      Oi::Proxy proxy("test");

      double** nodesarray = 0;
      int nnodes = 0;

      proxy.getNodes(nodesarray, nnodes);      
      if (nodesarray == 0 || nnodes == 0)
          return alloc_array(0);

      value arr = alloc_array(nnodes);
      for (int i = 0; i < nnodes; ++i)
      {
        value o1  = alloc_object(NULL);

        alloc_field(o1, val_id("x"), alloc_float(nodesarray[i][0]));
        alloc_field(o1, val_id("y"), alloc_float(nodesarray[i][1]));
        alloc_field(o1, val_id("z"), alloc_float(nodesarray[i][2]));
 

        val_array_ptr(arr)[i] = alloc_object(o1);
      }
    
      // free allocated memory.
      Oi::free2D(nodesarray, nnodes);     
      return arr;
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

static value getLines()
{
    Oi::Proxy proxy("test");
    
    double** linesarray = 0;
    int nlines = 0;

    proxy.getLines(linesarray, nlines);
    if (linesarray == 0 || nlines == 0)
        return alloc_array(0);

    value arr = alloc_array(nlines*2);

    for (int idx = 0; idx < nlines; ++idx)
    {
        value o1  = alloc_object(NULL);
        alloc_field(o1, val_id("x"), alloc_float(linesarray[idx][0]));
        alloc_field(o1, val_id("y"), alloc_float(linesarray[idx][1]));
        alloc_field(o1, val_id("z"), alloc_float(linesarray[idx][2]));
        
        value  o2 = alloc_object(NULL);
        alloc_field(o2, val_id("x"), alloc_float(linesarray[idx][3]));
        alloc_field(o2, val_id("y"), alloc_float(linesarray[idx][4]));
        alloc_field(o2, val_id("z"), alloc_float(linesarray[idx][5]));

        val_array_ptr(arr)[2*idx] = alloc_object(o1);
        val_array_ptr(arr)[2*idx+1] = alloc_object(o2);
    }

    Oi::free2D(linesarray, nlines);
    return arr;
}

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

    if (vFileNames.empty())
        return vFileNames;

    for (size_t i = 0; i < vFileNames.size(); ++i)
    {
        string file = vFileNames[i];
        string strDName;
        string::size_type idx = file.find('.');
        if (idx != string::npos)
        {
            strDName = file.substr(0, idx);
        }
        else 
            strDName = file;

        idx = strDName.rfind('/');
        if ( idx != string::npos)
            strDName = strDName.substr(idx+1, string::npos);
        
        vFileNames[i] = strDName;
    }

    return vFileNames;
}

value getSurfaces( value varr )
{
    vector<string> strNames = convert2string( varr );
    if (strNames.empty())
        return alloc_array(0);

    Oi::Proxy proxy("test");
    double** surfacearray = 0;
    int nsurfaces = 0;

    proxy.getSurfaces(surfacearray, nsurfaces);
    if ( surfacearray == 0 )
        return alloc_array(0);
    
    value arr = alloc_array(nsurfaces*3);
    
    for (int idx = 0; idx < nsurfaces; ++idx)
    {
        value  o1 = alloc_object(NULL);
        alloc_field(o1, val_id("x"), alloc_float(surfacearray[idx][0]));
        alloc_field(o1, val_id("y"), alloc_float(surfacearray[idx][1]));
        alloc_field(o1, val_id("z"), alloc_float(surfacearray[idx][2]));

        value  o2 = alloc_object(NULL);
        alloc_field(o2, val_id("x"), alloc_float(surfacearray[idx][3]));
        alloc_field(o2, val_id("y"), alloc_float(surfacearray[idx][4]));
        alloc_field(o2, val_id("z"), alloc_float(surfacearray[idx][5]));

        value  o3 = alloc_object(NULL);
        alloc_field(o3, val_id("x"), alloc_float(surfacearray[idx][6]));
        alloc_field(o3, val_id("y"), alloc_float(surfacearray[idx][7]));
        alloc_field(o3, val_id("z"), alloc_float(surfacearray[idx][8]));


        val_array_ptr(arr)[3*idx] = alloc_object(o1);
        val_array_ptr(arr)[3*idx+1] = alloc_object(o2);
        val_array_ptr(arr)[3*idx+2] = alloc_object(o3);
    }
   
    Oi::free2D(surfacearray, nsurfaces);
    return arr;
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

DEFINE_PRIM(getNodes, 0);
DEFINE_PRIM(getNumberOfNodes,0);
DEFINE_PRIM(getNumberOfLines, 0);
DEFINE_PRIM(getLines, 0);
DEFINE_PRIM(getSurfaces, 1);
DEFINE_PRIM(getPSD, 0);

DEFINE_PRIM(getNodeLocation, 1);
DEFINE_PRIM(getStatus, 0);
DEFINE_PRIM(say, 0);
DEFINE_PRIM(init, 1);

//DEFINE_PRIM_MULT(init);
