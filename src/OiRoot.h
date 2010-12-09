// =====================================================================================
// 
//       Filename:  OiRoot.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-29 11:02:16
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


#ifndef  _OIROOT_H
#define  _OIROOT_H

#include	"OiPersist.h"
#include	"OiProxy.h"
#include    <boost/shared_ptr.hpp>
#include    <vector>


using boost::shared_ptr;
using std::vector;

namespace Oi
{
    // forward declaration.
    class FileFormatInterface;
    class ProcessingInterface;
    class StorageInterface;

    class Root : public ProxyInterface, public PersistInterface 
    {
        private:
                Root();
                ~Root();
                Root(Root const&);
                Root& operator=(Root const&);

                static Root* instance_;
                string repositoryName_;

                shared_ptr<StorageInterface> storage_;
                shared_ptr<FileFormatInterface> fileFormat_;
                vector< shared_ptr<ProcessingInterface> > procList_;
            
            // ProxyInterface 
            public:
                bool init(int argc, const char** fileList, int processName = 0);
                bool connect(const string& repoName);
                double** getNodes(int& size);
                unsigned int** getLines(int& size);
                unsigned int** getSurfaces(int& size);
            
            // PersistInterface 
            public:
                void save();
                void load();
            
            public:
                static Root* Instance();
                
                shared_ptr<StorageInterface> getStorage();
                shared_ptr<FileFormatInterface> getFileFormat();
                shared_ptr<ProcessingInterface> getProcess(int i);

    };

} // namespace Oi


#endif   // ----- #ifndef OIROOT_INC  -----


