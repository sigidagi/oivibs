// =====================================================================================
// 
//       Filename:  OiUtil.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-07 11:52:20
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Sigitas Dagilis (), sigidagi@gmail.com
//        Company:  sigidagis
// 
// =====================================================================================

#include "OiUtil.h"

namespace Oi {

    void free2D(double** p2Darray, int length)
    {
        for (int i = 0; i < length; ++i)
            delete [] p2Darray[i];

        delete [] p2Darray;
        p2Darray = 0;
    }

} // namespace Oi
