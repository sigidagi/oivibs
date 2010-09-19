// =====================================================================================
// 
//       Filename:  OiSsiProcessing.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-13 12:44:48
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Sigitas Dagilis (), sigidagi@gmail.com
//        Company:  sigidagis
// 
// =====================================================================================
#ifndef _OISSIPROCESSING_H
#define _OISSIPROCESSING_H

#include "OiProcessing.h"

namespace Oi 
{
    class FileFormatInterface;
    
    class SsiProcessing : public ProcessingInterface 
    {
        public:
            SsiProcessing(StorageInterface* owner);
            ~SsiProcessing();

         public:
            bool start();

    };

} // namespace Oi

#endif
