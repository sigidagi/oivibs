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

#include    "config.hpp"
#include	"OiRoot.h"
#include	"OiFileFormat.h"
#include	"OiProcessing.h"
#include	"OiUtil.h"
#include    <boost/foreach.hpp>
#include	<boost/bind.hpp>

#define     foreach BOOST_FOREACH

namespace Oi
{

    Root* Root::instance_ = NULL;
    
    Root::Root()
    {

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
           
            string::size_type idx = strInFile.rfind('/');
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
        // 
        foreach(string file, fileList)
        {
            shared_ptr<FileFormatInterface> fileFormat = FileFormatInterface::createFileFormat(this, file);
            fileFormat->parse();
            if (fileFormat->existNodes() || 
                fileFormat->existLines() ||
                fileFormat->existSurfaces() ||
                fileFormat->existChannels() )
            {
                fileFormatList_.push_back(fileFormat);
            }
            
            if (fileFormat->existChannels())
            {
                shared_ptr<ProcessingInterface> proc = ProcessingInterface::createProcess(processName, file);
                if (proc->start(fileFormat.get()))
                    procList_.push_back(proc);
 
            }
            

        } // end of foreach in fileList
        
        if (fileFormatList_.empty())
            return false;

        return true;
    }

    bool Root::connect(const string& file)
    {
/*
 *        string repoName = Oi::stripToBaseName(file);
 *        if (!storage_->existRepository(repoName))
 *            return false;
 *
 *        repositoryName_ = file;
 */

        return true;
    }
    
    const double* Root::getNodes(int& nrows, int& ncols) const
    {
        if (fileFormatList_.empty())
            return NULL;
       
        foreach(shared_ptr<FileFormatInterface> format, fileFormatList_)
        {
            if (format->existNodes())
                return format->getNodes(nrows, ncols);
        }
        return NULL;
    }

    const unsigned int* Root::getLines(int& nrows, int& ncols) const
    {
        if (fileFormatList_.empty())
            return NULL;
       
        foreach(shared_ptr<FileFormatInterface> format, fileFormatList_)
        {
            if (format->existLines())
                return format->getLines(nrows, ncols);
        }
        return NULL;
    }
    
    const unsigned int* Root::getSurfaces(int& nrows, int& ncols) const
    {
        if (fileFormatList_.empty())
            return NULL;
       
        foreach(shared_ptr<FileFormatInterface> format, fileFormatList_)
        {
            if (format->existSurfaces())
                return format->getSurfaces(nrows, ncols);
        }
        return NULL;
    }

    const double* Root::getSingularValues(const string& fileName, int& nrows, int& ncols) const 
    {
        shared_ptr<ProcessingInterface> pt = getProcess(fileName);
        if (pt.get() == 0)
            return NULL;
        
        return pt->getSingularValues(nrows, ncols);
    }

    shared_ptr<FileFormatInterface> Root::getFileFormat() 
    {
        assert(false);
        return shared_ptr<FileFormatInterface>();
    }
   
    shared_ptr<ProcessingInterface> Root::getProcess(const string& fileName) const 
    {
        string file = Oi::stripToFileName(fileName);

        vector<string> coll;
        std::transform(procList_.begin(), 
                       procList_.end(), 
                       back_inserter(coll), 
                       boost::bind(&ProcessingInterface::getFileName, _1));
    
        vector<string>::const_iterator cit;
        vector< shared_ptr<ProcessingInterface> >::const_iterator proc_it;
        
        cit = find(coll.begin(), coll.end(), file);
        if (cit != coll.end())
        {
            int idx = (int)(cit - coll.begin());
            return procList_[idx]; 
        }
        else
            return shared_ptr<ProcessingInterface>();
    }


} // namespace Oi
