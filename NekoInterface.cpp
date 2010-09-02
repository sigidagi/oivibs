#include "UniversalFormat.h"
#include <iostream>
#include <neko.h>

#include "OiDatabase.h"
#include "OiGeometry.h"
#include "OiData.h"

value init( value varr )
{
    int sz = val_array_size(varr);

    if (sz == 0)
        return val_false;

    vector<string> vFileNames;

    for (int i = 0; i < sz; ++i)
    {
        if (val_is_string(val_array_ptr(varr)[i]))
            vFileNames.push_back( string(val_string(val_array_ptr(varr)[i])) );
    }

    if (vFileNames.empty())
        return val_false;
    
	UniversalFormat uff(vFileNames);
    // search for goemetry: nodes, lines and surfaces and search for data.
    uff.parse();

    if ( uff.existData())
    {
        OiData oid;
        oid.startProcessing(&uff);

    }
	    if ( !uff.existNodes() )
        return val_false;

    if ( !uff.existLines() && !uff.existSurfaces() )
        return val_false;

    return val_true;
}

value getPSD()
{
/*
 *    const mat mPSD = OiData::Instance().getPSD();
 *    
 *    int nrows = mPSD.n_rows;
 *
 *    value arr = alloc_array((double)nrows);
 *
 *    for (int i = 0; i < nrows; ++i)
 *    {
 *        val_array_ptr(arr)[i] = alloc_float(mPSD(i,0));
 *    }
 *
 *    return arr;
 */
    return val_null;
}

value getNodes()
{
    OiDatabase dbase;
    mysqlpp::Connection conn;
    dbase.connect("test", conn);
    
    mysqlpp::Query query = conn.query("select * from geonodes");
    mysqlpp::StoreQueryResult res = query.store();

    value arr = alloc_array((int)res.num_rows()*3);

	for (size_t i = 0; i < res.num_rows()*3; i+=3)
    {
		val_array_ptr(arr)[i] = alloc_int(res[i/3]["x"]);
		val_array_ptr(arr)[i+1] = alloc_int(res[i/3]["y"]);
		val_array_ptr(arr)[i+2] = alloc_int(res[i/3]["z"]);
    }

	return arr;


/*
 *    vector<int> vNodes;
 *    OiGeometry::Instance().getNodes(vNodes);
 *
 *    if (vNodes.empty())
 *        return alloc_array(0);
 *
 *    value arr = alloc_array((int)vNodes.size());
 *
 *    for (size_t i = 0; i < vNodes.size(); ++i)
 *        val_array_ptr(arr)[i] = alloc_int(vNodes[i]);
 *
 *    return arr;
 */
}

static value getNumberOfNodes()
{
    OiDatabase dbase;
    mysqlpp::Connection conn;
    dbase.connect("test", conn);
    
    mysqlpp::Query query = conn.query("select * from geonodes");
    mysqlpp::StoreQueryResult res = query.store();

    value nnodes = alloc_int((int)res.num_rows());
	
	return nnodes;
}

static value getNodeLocation(value iNode)
{
	value o = alloc_object(NULL);

	if (!val_is_int(iNode))
		return o;

	Point3D pt = OiGeometry::Instance().getNodeLocation(val_int(iNode));
	
	alloc_field(o, val_id("x"), alloc_float(pt.x));
	alloc_field(o, val_id("y"), alloc_float(pt.y));
	alloc_field(o, val_id("z"), alloc_float(pt.z));

	return o;
}

static value getLines()
{
    mysqlpp::Connection conn;
    OiDatabase dbase;
    bool bRet = dbase.connect("test", conn);
    if (bRet == false)
        return val_null;


    mysqlpp::Query query_nodes = conn.query("select * from geonodes");
    mysqlpp::StoreQueryResult resNodes = query_nodes.store(); 

    mysqlpp::Query query_lines = conn.query("select * from geolines");
    mysqlpp::StoreQueryResult resLines = query_lines.store();
    
    
    if (!resLines || !resNodes)
        return val_null;

    int nLines = (int)resLines.num_rows();
    value arr = alloc_array(nLines*2);
    double x1,y1,z1,x2,y2,z2;
    int node1, node2;

    for (int idx = 0; idx < nLines*2; idx+=2)
    {
        node1 = resLines[idx/2]["node1"];
        x1 = resNodes[node1-1]["x"];
        y1 = resNodes[node1-1]["y"];
        z1 = resNodes[node1-1]["z"];
       
        value o1  = alloc_object(NULL);

        alloc_field(o1, val_id("x"), alloc_float(x1));
        alloc_field(o1, val_id("y"), alloc_float(y1));
        alloc_field(o1, val_id("z"), alloc_float(z1));
        
        node2 = resLines[idx/2]["node2"];
        x2 = resNodes[node2-1]["x"];
        y2 = resNodes[node2-1]["y"];
        z2 = resNodes[node2-1]["z"];
 
        value  o2 = alloc_object(NULL);
        alloc_field(o2, val_id("x"), alloc_float(x2));
        alloc_field(o2, val_id("y"), alloc_float(y2));
        alloc_field(o2, val_id("z"), alloc_float(z2));

        val_array_ptr(arr)[idx] = alloc_object(o1);
        val_array_ptr(arr)[idx+1] = alloc_object(o2);
    }
    
    return arr;
   
/*
 *    int nLines = OiGeometry::Instance().getNumberOfLines();
 *    if (nLines == 0 )
 *        return val_null;
 *
 *    // As each line consist of two points in the 3D space, so number of points is 
 *    // equal number of lines mult. by 2.
 *    // Return value will be points, where line start will be event point, end - odd point.
 *    value arr = alloc_array(nLines*2);
 *    for (int idx = 0; idx < nLines*2; idx+=2)
 *    {
 *        Line line = OiGeometry::Instance().getLine(idx/3);
 *        Point3D pt1 = OiGeometry::Instance().getNodeLocation( line.node1 );
 *        Point3D pt2 = OiGeometry::Instance().getNodeLocation( line.node2 );
 *
 *        value  o1 = alloc_object(NULL);
 *        alloc_field(o1, val_id("x"), alloc_float(pt1.x));
 *        alloc_field(o1, val_id("y"), alloc_float(pt1.y));
 *        alloc_field(o1, val_id("z"), alloc_float(pt1.z));
 *
 *        value  o2 = alloc_object(NULL);
 *        alloc_field(o2, val_id("x"), alloc_float(pt2.x));
 *        alloc_field(o2, val_id("y"), alloc_float(pt2.y));
 *        alloc_field(o2, val_id("z"), alloc_float(pt2.z));
 *
 *        val_array_ptr(arr)[idx] = alloc_object(o1);
 *        val_array_ptr(arr)[idx+1] = alloc_object(o2);
 *    }
 *    
 *    return arr;
 */
}

value getSurfaces( value varr )
{
    int sz = val_array_size(varr);

    if (sz == 0)
        return val_null;

    vector<string> vFileNames;

    for (int i = 0; i < sz; ++i)
    {
        if (val_is_string(val_array_ptr(varr)[i]))
            vFileNames.push_back( string(val_string(val_array_ptr(varr)[i])) );
    }

    if (vFileNames.empty())
        return val_null;

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

 
        mysqlpp::Connection conn;
        OiDatabase dbase;
        bool bRet = dbase.connect(strDName, conn);
        if (bRet == false)
            continue;

    
        mysqlpp::Query query = conn.query("select * from surfaces");
        mysqlpp::StoreQueryResult resSurfaces = query.store();
        if (!resSurfaces)
            continue;

        query.reset();
        query = conn.query("select * from geonodes");
        mysqlpp::StoreQueryResult resNodes = query.store(); 
        if (!resNodes)
            continue;

        
        int nSurfaces = (int)resSurfaces.num_rows();
        value arr = alloc_array(nSurfaces*3);
        double x1,y1,z1,x2,y2,z2,x3,y3,z3;
        
        for (int idx = 0; idx < nSurfaces*3; idx+=3)
        {
            
            x1 = resNodes[resSurfaces[idx/3]["node1"]]["x"];
            y1 = resNodes[resSurfaces[idx/3]["node1"]]["y"];
            z1 = resNodes[resSurfaces[idx/3]["node1"]]["z"];
            
            value  o1 = alloc_object(NULL);
            alloc_field(o1, val_id("x"), alloc_float(x1));
            alloc_field(o1, val_id("y"), alloc_float(y1));
            alloc_field(o1, val_id("z"), alloc_float(z1));

            x2 = resNodes[resSurfaces[idx/3]["node2"]]["x"];
            y2 = resNodes[resSurfaces[idx/3]["node2"]]["y"];
            z2 = resNodes[resSurfaces[idx/3]["node2"]]["z"];
     
            value  o2 = alloc_object(NULL);
            alloc_field(o2, val_id("x"), alloc_float(x2));
            alloc_field(o2, val_id("y"), alloc_float(y2));
            alloc_field(o2, val_id("z"), alloc_float(z2));

            x3 = resNodes[resSurfaces[idx/3]["node3"]]["x"];
            y3 = resNodes[resSurfaces[idx/3]["node3"]]["y"];
            z3 = resNodes[resSurfaces[idx/3]["node3"]]["z"];
     
            value  o3 = alloc_object(NULL);
            alloc_field(o3, val_id("x"), alloc_float(x3));
            alloc_field(o3, val_id("y"), alloc_float(y3));
            alloc_field(o3, val_id("z"), alloc_float(z3));


            val_array_ptr(arr)[idx] = alloc_object(o1);
            val_array_ptr(arr)[idx+1] = alloc_object(o2);
            val_array_ptr(arr)[idx+2] = alloc_object(o3);
        }
        
        return arr;
    }
    
    return val_null;
}
static value getNumberOfLines()
{

    OiDatabase dbase;
    mysqlpp::Connection conn;
    dbase.connect("test", conn);
    
    mysqlpp::Query query = conn.query("select * from geolines");
    mysqlpp::StoreQueryResult res = query.store();

    return alloc_int((int)res.num_rows());
}

static value getStatus()
{
    vector<string> vstr = OiGeometry::Instance().getStatus();
    if (vstr.empty())
    {
        value arr = alloc_array(1);
        val_array_ptr(arr)[0] = alloc_string("Status is empty");

        return arr;   
    }
   
    value arr = alloc_array( (int)vstr.size() );
    for (size_t t = 0; t < vstr.size(); ++t)
    {
        string str = vstr[t];
        val_array_ptr(arr)[t] = alloc_string( str.c_str() );
    }

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
