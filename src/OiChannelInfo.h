// =====================================================================================
// 
//       Filename:  OiChannelInfo.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-02-06 12:25:33
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


#ifndef  OICHANNELINFO_INC
#define  OICHANNELINFO_INC

struct ChannelInfo 
{
    string name;
    unsigned int node;
    int direction;
    unsigned int nsamples;
    double sampling;
};

#endif   // ----- #ifndef OICHANNELINFO_INC  -----
