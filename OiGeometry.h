#ifndef _OIGEOMETRY_H
#define _OIGEOMETRY_H

#include <ostream>
#include <vector>
#include <string>
#include <map>

using std::ostream;
using std::map;
using std::vector;
using std::string;

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

ostream& operator<<(ostream& stream, Point3D& ob);
ostream& operator<<(ostream& stream, Line& ob);
ostream& operator<<(ostream& stream, Surface& ob);


class OiGeometry
{
private:
	OiGeometry(){};
	OiGeometry(OiGeometry const&){};
	OiGeometry& operator=(OiGeometry const&){ return *this;};

	map<int,Point3D> m_vNodes;
	vector< Surface > m_vSurfaces;
	vector< Line > m_vLines;
    
    vector<string> __strStatus;

	friend class UniversalFormat;

public:

	static OiGeometry& Instance();

	void getNodes(vector<int>& vNodes);
	Point3D getNodeLocation(int nNode);

	int getNumberOfLines();
	Line getLine(int index);

	int getNumberOfSurfaces();
	Surface getSurface(int index);

    vector<string> getStatus();
    void clearStatus();
};

#endif
