// =====================================================================================
// 
//       Filename:  OiRoot.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-29 11:17:54
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Sigitas Dagilis (), sigidagi@gmail.com
//        Company:  sigidagis
// 
// =====================================================================================


#include	"OiRoot.h"

namespace Oi
{
    Root* Root::instance_ = NULL;
    
    Root::Root()
    {
        #if defined(OI_USE_MYSQLPP)
        storage_ = shared_ptr<StorageInterface>(new DatabaseStorage());
        #else
        storage_ = shared_ptr<StorageInterface>(new LocalStorage());
        #endif

    }

    Root::~Root()
    {

    }

    Root& Root::operator=(Root const&)
    {
        return *this;
    }

    Root* Root::Instance()
    {
        if (!instance_)
            instance_ = new Root;
        
        return instance_;
    }

    shared_ptr<StorageInterface> Root::getStorage()
    {
        return storage_;
    }
    
    void Root::save(const string& name)
    {
        fileFormat_->save(name);
        proc_->save(name);
    }

    void Root::load(const string& name)
    {
        fileFormat_->load(name);
        proc_->load(name);
    }
    
    // Implementation of ProxyInterface interface

    bool Root::init(const string& file, int processName)
    {
         // 
        fileFormat_ = FileFormatInterface::createFileFormat(this, file);
        fileFormat_->parse(file);

        proc_ = ProcessingInterface::createProcess(this, processName);
        if (proc_->start())
        {
               // save processed data, singular values, singular vectors and etc.
        }
        
        // Root is responsible for initialization of Storage
        string repoName = Oi::stripToBaseName(file);
        if (repoName.empty())
            return;

        storage_->init(repoName);

        return true;
    }

    bool Root::connect(const string file)
    {
        // NOT implemented jet!
        return false;
    }
    
    double** Root::getNodes(int& size)
    {
        if (fileFormat_.get() == NULL)
            return NULL;

         // first check if nodes are stored in variable 
        const arma::mat& nodes = fileFormat_->getNodes();
        if (nodes.n_elem != 0 && nodes.n_rows == 4)
        {
            double** array = new double*[size];
            for (int n = 0; n < size; ++n)
                array[n] = new double[3];

            // assign values
            for (size_t i = 0; i < nodes.n_cols; ++i)
            {
                array[i][0] = nodes(1, i);
                array[i][1] = nodes(2, i);
                array[i][2] = nodes(3, i);
            }
            
            return array;
        }

    double** Root::getLines(int& size)
    {
        // NOT implemented jet.
        return NULL;
    }
    
    double** Root::getSurfaces(int& size)
    {
        // NOT impolemented jet!
        return NULL;
    }

    shared_ptr<FileFormatInterface> Root::getFileFormat() 
    {
        return fileFormat_;
    }
   
    shared_ptr<ProcessingInterface> Root::getProcess() 
    {
        return proc_; 
    }


} // namespace Oi
