// =====================================================================================
// 
//       Filename:  geometry.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-02-02 12:30:37
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Sigitas Dagilis (), sigidagi@gmail.com
//
//     This file is part of the OiVibrations C++ library.
//     It is provided without any warranty of fitness
//     for any purpose. You can redistribute this file
//     and/or modify it under the terms of the GNU
//     General Public License (GPL) as published
//     by the Free Software Foundation, either version 3
//     of the License or (at your option) any later version.
//     (see http://www.opensource.org/licenses for more info)
// 
// =====================================================================================

#ifndef  GEOMETRY_INC
#define  GEOMETRY_INC


#include	<iostream>
#include	<iomanip>

struct Point
{
    Point() {}
    Point(double xx, double yy, double zz) : x(xx), y(yy), z(zz) {}
    double x;
    double y;
    double z;
    
    friend std::ostream& operator<<(std::ostream& os, Point pt);
};

std::ostream& operator<<(std::ostream& os, Point pt)
{
    os.setf(std::ios::left, std::ios::adjustfield);
    os << "(" << std::setw(6) << pt.x << std::setw(6) << pt.y;

    os.setf(std::ios::right, std::ios::adjustfield);
    os << std::setw(3) << pt.z << ")";
    return os;
}

struct Line
{
    Line() {}
    Line(Point pt1, Point pt2) : point1(pt1), point2(pt2) {}
    Point point1;
    Point point2;

    friend std::ostream& operator<<(std::ostream& os, Line line);
};

std::ostream& operator<<(std::ostream& os, Line line)
{
    os << line.point1 << "  " << line.point2; 
    return os;
}


struct Surface
{
    Surface() {}
    Surface(Point pt1, Point pt2, Point pt3) : point1(pt1), point2(pt2), point3(pt3) {}
    Point point1;
    Point point2;
    Point point3;

    friend std::ostream& operator<<(std::ostream& os, Surface surface);
};

std::ostream& operator<<(std::ostream& os, Surface surface)
{
    os << surface.point1 << "  " << surface.point2 << "  " << surface.point3;
    return os;
}

#endif   // ----- #ifndef GEOMETRY_INC  -----
