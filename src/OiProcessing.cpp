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
    
    shared_ptr<ProcessingInterface> ProcessingInterface::createProcess(Root* owner, int processName)
    {
        if (processName == Process::FDD)
        {
            return shared_ptr<ProcessingInterface>(new FddProcessing(owner));
        }
        else if (processName == Process::SSI)
        {
            return shared_ptr<ProcessingInterface>(new SsiProcessing(owner));
        }
        // default process.
        else
        {
            return shared_ptr<ProcessingInterface>(new FddProcessing(owner));
        }
    }
   
    void ProcessingInterface::save()
    {

    }
    
    void ProcessingInterface::load()
    {

    }

} // namespace Oi

