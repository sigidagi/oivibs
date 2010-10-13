// =====================================================================================
// 
//       Filename:  OiSsiProcessing.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-13 12:48:02
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Sigitas Dagilis (), sigidagi@gmail.com
//        Company:  sigidagis
// 
// =====================================================================================

#include "OiSsiProcessing.h"
#include "OiRoot.h"

namespace Oi {

    SsiProcessing::SsiProcessing(Root* owner) : ProcessingInterface(owner)
    {

    }

    SsiProcessing::~SsiProcessing()
    {

    }

    bool SsiProcessing::start()
    {
        // not implemented jet.
        return false;
    }

} // namespace Oi


