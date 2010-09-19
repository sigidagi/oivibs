// =====================================================================================
// 
//       Filename:  OiStorage.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-17 15:45:15
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Sigitas Dagilis (), sigidagi@gmail.com
//        Company:  sigidagis
// 
// =====================================================================================

#include "OiStorage.h"

namespace Oi 
{
    FileFormatInterface* StorageInterface::getFileFormat()
    {
        return fileFormat_;
    }
   
    ProcessingInterface* StorageInterface::getProcess()
    {
        return proc_; 
    }

} // namespace Oi
