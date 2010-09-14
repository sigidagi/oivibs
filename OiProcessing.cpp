// =====================================================================================
// 
//       Filename:  OiProcessing.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-13 12:02:48
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Sigitas Dagilis (), sigidagi@gmail.com
//        Company:  sigidagis
// 
// =====================================================================================

#include "OiProcessing.h"
#include "OiFddProcessing.h"
#include "OiSsiProcessing.h"
#include "OiUtil.h"

namespace Oi {

    ProcessingInterface* ProcessingInterface::createProcess(int processName)
    {
        if (processName == FDD)
        {
            return new FddProcessing;
        }
        else if (processName == SSI)
        {
            return new SsiProcessing;
        }
        // default process.
        else
        {
            return new FddProcessing;
        }
    }
    
} // namespace Oi

