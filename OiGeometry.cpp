#include "OiGeometry.h"


ostream& operator<<(ostream& stream, Point3D& ob)
{
	stream << "[" << ob.x << " " << ob.y << " " << ob.z << "]" << std::endl;
	return stream;
}

ostream& operator<<(ostream& stream, Line& ob)
{
	stream << "[" << ob.node1 << " " << ob.node1 << "]" << " ";
	return stream;
}

ostream& operator<<(ostream& stream, Surface& ob)
{
	stream << "[" << ob.node1 << " " << ob.node2 << " " << ob.node3 << "]" << " "; 
	return stream;
}


OiGeometry& OiGeometry::Instance()
{
	static OiGeometry thisOiGeometry;
	return thisOiGeometry;
}

void OiGeometry::getNodes(vector<int>& vNodes)
{
	vNodes.resize(m_vNodes.size());
	map<int,Point3D>::iterator it;
	
	int count = 0;
	for (it = m_vNodes.begin(); it != m_vNodes.end(); ++it)
	{
		vNodes[count] = (*it).first;
		++count;
	}
}

Point3D OiGeometry::getNodeLocation(int nNode)
{
	map<int, Point3D>::iterator it;
	it = m_vNodes.find(nNode);
	if (it == m_vNodes.end())
	{
		Point3D pt;
		pt.x = pt.y = pt.z = -1;
		return pt;
	}

	return it->second;
}

int OiGeometry::getNumberOfLines()
{
	return (int)m_vLines.size();
}

Line OiGeometry::getLine(int index)
{
	if ((int)m_vLines.size() <= index )
	{
		Line li;
		li.node1 = -1;
		li.node2 = -1;
		return li;
	}

	return m_vLines[index];
}

int OiGeometry::getNumberOfSurfaces()
{
	return (int)m_vSurfaces.size();
}

Surface OiGeometry::getSurface(int index)
{
	if ( (int)m_vSurfaces.size() <= index )
	{
		Surface sr;
		sr.node1 = sr.node2 = sr.node3 = -1;
		return sr;
	}

	return m_vSurfaces[index];
}

vector<string> OiGeometry::getStatus()
{
    return __strStatus;
}

void OiGeometry::clearStatus()
{
    __strStatus.clear();
}

