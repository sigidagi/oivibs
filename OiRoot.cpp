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
#include	"OiDatabaseStorage.h"
#include	"OiLocalStorage.h"
#include	"OiFileFormat.h"
#include	"OiProcessing.h"
#include	"OiUtil.h"

namespace Oi
{
    Root* Root::instance_ = NULL;
    
    Root::Root()
    {
        #if defined(OI_USE_MYSQLPP)
        storage_ = shared_ptr<StorageInterface>(new DatabaseStorage);
        #else
        storage_ = shared_ptr<StorageInterface>(new LocalStorage);
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
            return false;

        storage_->init(repoName);

        return true;
    }

    bool Root::connect(const string& file)
    {
        string repoName = Oi::stripToBaseName(file);
        if (!storage_->existRepository(repoName))
            return false;

        repositoryName_ = file;

        return true;
    }
    
    double** Root::getNodes(int& size)
    {
        if (fileFormat_.get() == NULL)
            return NULL;

        // first check if nodes are stored in variable 
        // if variable is empty try to load from repository 
        if (!fileFormat_->existNodes())
        {
            fileFormat_->load(repositoryName_);
            if (!fileFormat_->existNodes())
                return NULL;
        }

        // if we reached this point - nodes exist and can allocate matrix for user.
        arma::mat& nodes = fileFormat_->getNodes();
        size = (int)nodes.n_rows;
        
        return allocate2D(nodes); 
    }

    unsigned int** Root::getLines(int& size)
    {
         if (fileFormat_.get() == NULL)
            return NULL;

        // first check if lines are stored in variable 
        // if variable is empty try to load from repository 
        if (!fileFormat_->existLines())
        {
            fileFormat_->load(repositoryName_);
            if (!fileFormat_->existLines())
                return NULL;
        }

        // if we reached this point - nodes exist and can allocate matrix for user.
        arma::umat& lines = fileFormat_->getLines();
        size = (int)lines.n_rows;
        
        return allocate2D(lines); 
    }
    
    unsigned int** Root::getSurfaces(int& size)
    {
          if (fileFormat_.get() == NULL)
            return NULL;

        // first check if nodes are stored in variable 
        // if variable is empty try to load from repository 
        if (!fileFormat_->existSurfaces())
        {
            fileFormat_->load(repositoryName_);
            if (!fileFormat_->existSurfaces())
                return NULL;
        }

        // if we reached this point - nodes exist and can allocate matrix for user.
        arma::umat& surfaces = fileFormat_->getSurfaces();
        size = (int)surfaces.n_rows;
        
        return allocate2D(surfaces); 
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
