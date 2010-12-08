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

#include    "config.hpp"
#include	"OiRoot.h"

#if defined(OI_USE_MYSQLPP)
#include	"OiDatabaseStorage.h"
#endif

#include	"OiLocalStorage.h"
#include	"OiFileFormat.h"
#include	"OiProcessing.h"
#include	"OiUtil.h"
#include    <boost/foreach.hpp>

#define     foreach BOOST_FOREACH

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
    
    void Root::save()
    {
        fileFormat_->save();
        foreach( shared_ptr<ProcessingInterface> proc, procList_)
            proc->save();
    }

    void Root::load()
    {
        fileFormat_->load();
        foreach( shared_ptr<ProcessingInterface> proc, procList_)
            proc->load();
    }
    
    // Implementation of ProxyInterface interface

    bool Root::init(int argc, const char** argv, int processName)
    {
        
        vector<string> fileList;
        for (int i = 0; i < argc; ++i)
        {
            // check if file format is supported.
            string strInFile = argv[i];
            string fileFormat = stripToExtension(strInFile);
            if (fileFormat != "uff" && fileFormat != "txt")
                continue;
           
            
            string::size_type idx = strInFile.rfind('\\');
            if (idx != string::npos)
            {
                string strPath = strInFile.substr(0, idx);
                
                chdir(strPath.c_str());
                strInFile = strInFile.substr(idx+1);
            }

            fileList.push_back(strInFile);
        }
        
        if (fileList.empty())
            return false;
             

        // Root is responsible for initialization of Storage
        string repoName = Oi::stripToBaseName(fileList[0]);
        if (repoName.empty())
            return false;
        // 
        storage_->init(repoName);
        // 
        
        foreach(string file, fileList)
        {
            fileFormat_ = FileFormatInterface::createFileFormat(this, file);
            fileFormat_->parse(file);

            shared_ptr<ProcessingInterface> proc = ProcessingInterface::createProcess(this, processName);
            if (proc->start())
            {
                   procList_.push_back(proc);

                   // save processed data, singular values, singular vectors and etc.
            }
        }

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
            fileFormat_->load();
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
            fileFormat_->load();
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
            fileFormat_->load();
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
   
    shared_ptr<ProcessingInterface> Root::getProcess(int i) 
    {
        if (i >= 0 && i < (int)procList_.size())
            return  procList_[i]; 
        else
            return shared_ptr<ProcessingInterface>();
    }


} // namespace Oi
