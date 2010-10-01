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
//        Company:  sigidagis
// 
// =====================================================================================


#ifndef  _OIROOT_H
#define  _OIROOT_H

#include	"OiPersist.h"
#include	"OiProxy.h"
#include    <armadillo>
#include    <boost/shared_ptr.hpp>

using boost::shared_ptr;

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
                shared_ptr<ProcessingInterface> proc_;
            
            private:
                template<typename T>
                T** allocate2D(arma::Mat<T> a)
                {
                    if (a.n_elem == 0 || a.n_rows == 0) 
                        return NULL;

                    T** array = new T*[a.n_rows];
                    for (size_t n = 0; n < a.n_rows; ++n)
                        array[n] = new T[a.n_cols];

                    // assign values
                    for (size_t i = 0; i < a.n_rows; ++i)
                    {
                        for (size_t j = 0; j < a.n_cols; ++j)
                            array[i][j] = a(i,j);
                    }
                    
                    return array;
                }
        
            // ProxyInterface 
            public:
                bool init(const string& file, int processName = 0);
                bool connect(const string& repoName);
                double** getNodes(int& size);
                unsigned int** getLines(int& size);
                unsigned int** getSurfaces(int& size);
            
            // PersistInterface 
            public:
                void save(const string& name);
                void load(const string& name);
            
            public:
                static Root* Instance();
                
                shared_ptr<StorageInterface> getStorage();
                shared_ptr<FileFormatInterface> getFileFormat();
                shared_ptr<ProcessingInterface> getProcess();

    };

} // namespace Oi


#endif   // ----- #ifndef OIROOT_INC  -----


