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
#include "OiRoot.h" 
#include "OiFddProcessing.h"
#include "OiSsiProcessing.h"
#include "OiUtil.h"

namespace Oi {

    ProcessingInterface::ProcessingInterface(Root* owner) : root_(owner)
    {

    }
    
    ProcessingInterface::~ProcessingInterface()
    {

    }
    
    auto_ptr<ProcessingInterface> ProcessingInterface::createProcess(Root* owner, int processName)
    {
        if (processName == FDD)
        {
            return auto_ptr<ProcessingInterface>(new FddProcessing(owner));
        }
        else if (processName == SSI)
        {
            return auto_ptr<ProcessingInterface>(new SsiProcessing(owner));
        }
        // default process.
        else
        {
            return auto_ptr<ProcessingInterface>(new FddProcessing(owner));
        }
    }
    
} // namespace Oi

