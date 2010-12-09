// =====================================================================================
// 
//       Filename:  OiProcessing.h
// 
//    Description: 
// 
//        Version:  1.0
//        Created:  2010-09-07 12:16:18
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


#ifndef _OIPROCESSING_H
#define _OIPROCESSING_H


#include	"OiPersist.h"
#include    <boost/shared_ptr.hpp>

using boost::shared_ptr;

namespace Oi 
{
    class Root;

    class ProcessingInterface : public PersistInterface
    {
        public:
            ProcessingInterface(Root* owner);
            virtual ~ProcessingInterface();

        public:
            static shared_ptr<ProcessingInterface> createProcess(Root* owner, int processName);
            virtual bool start() = 0;

            // PersistInterface
            void save();
            void load();

        protected:
            Root* root_;
    };

} // namespace Oi

#endif
