#ifndef _OIGEOMETRY_H
#define _OIGEOMETRY_H

#include <ostream>
#include <vector>
#include <string>
#include <map>

#include "OiProxy.h"

using std::ostream;
using std::map;
using std::vector;
using std::string;


namespace Oi {

// Sensors (Nodes) placed on 3D space.
struct Point3D 
{
	double x;
	double y;
	double z;
};

struct Line
{
	int node1;
	int node2;
};

struct Surface
{
	int node1;
	int node2;
	int node3;
};

class OiGeometry : ProxyBase
{
    private:
        OiGeometry(){};
        OiGeometry(OiGeometry const&){};
        OiGeometry& operator=(OiGeometry const&){ return *this;};

        /*
         *map<int,Point3D> m_vNodes;
         *vector< Surface > m_vSurfaces;
         *vector< Line > m_vLines;
         *
         */

        vector<string> __strStatus;
//        static OiGeometry* instance_;
        
        friend class UniversalFormat;

    public:
        bool init(const string name); 
        void getNodes(double** array, int& nnnodes);
        void getLines(double** array, int& nlines);
        void getSurfaces(double** array, int& nSurfaces);

        static OiGeometry& Instance();

        vector<string> getStatus();
        void clearStatus();
};


} // namespace Oi


#endif
