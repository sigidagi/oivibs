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

using boost::bind;

namespace Oi
{

    Root* Root::instance_ = NULL;
    
    Root::Root() : processId_(-1) {}

    Root::~Root() {}

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

    bool Root::init(int argc, const char** argv, int processId)
    {
        fileList_.clear();

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

            fileList_.push_back(strInFile);
        }
        
        if (fileList_.empty())
            return false;
             
        // Root is responsible for initialization of Storage
        // 
        foreach(string file, fileList_)
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
                shared_ptr<ProcessingInterface> proc = ProcessingInterface::createProcess(processId, file);
                if (proc.get() == 0) 
                {
                    std::cerr << "Process name is incorrect!\n";
                    continue;
                }

                processId_ = processId;
               
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
   
    bool Root::selectProcess(int processId) 
    {
        if (procList_.empty())
            return false;
        
        foreach(shared_ptr<ProcessingInterface> proc, procList_) 
        {
            // check if exist process with such id.
            if (proc->getProcessId() == processId)
            {
                processId_ = processId;
                return true;
            }
        }
        return false;
    }
    
    bool Root::applyProcess(int processId)
    {
        // check if exist channel data
        if (fileFormatList_.empty())
            return false;
        
        // check if process with such processId exist. If yes - there is no need to do it again.
        if (selectProcess(processId))
            return true;

        // check if exist channel data
        bool exitData = false;
        foreach(shared_ptr<FileFormatInterface> format, fileFormatList_)
        {
            if (format->existChannels())
            {
                shared_ptr<ProcessingInterface> proc = ProcessingInterface::createProcess(processId, format->getFileName() );
                // check if processId is valid
                if (proc.get() == 0)
                    return false;
                
                // do processing and append to the list processed data.
                if (!proc->start(format.get()))
                    return false;
                
                procList_.push_back(proc);
                exitData = true;
            }
        }
        return exitData;
    }
    
    int Root::getNumberOfMeasurements() const
    {
        if (procList_.empty())
            return 0;
        
        vector<int> idList;
        std::transform(procList_.begin(),
                       procList_.end(),
                       back_inserter(idList),
                       boost::bind(&ProcessingInterface::getProcessId, _1));

        return std::count(idList.begin(), idList.end(), processId_);
    }
    
    int Root::getNumberOfChannels(unsigned int measurementNumber) const
    {
        shared_ptr<FileFormatInterface> pt = getFileFormat(measurementNumber);
        if (pt.get() == 0)
            return 0;
        
        return pt->getNumberOfChannels();
    }

    const double* Root::getSpectralDensity(unsigned int measurementNumber, int& nsamples, int& nchannels) const
    {
        shared_ptr<ProcessingInterface> pt = getProcess(processId_, measurementNumber);
        if (pt.get() == 0)
            return NULL;
        
        return pt->getSpectralDensity(nsamples, nchannels);
    }

    const double* Root::getSingularValues(unsigned int measurementNumber, int& nrows, int& ncols) const 
    {
        // current (selected or by default) process.
        shared_ptr<ProcessingInterface> pt = getProcess(processId_, measurementNumber);
        if (pt.get() == 0)
            return NULL;
        
        return pt->getSingularValues(nrows, ncols);
    }

    const double* Root::getFrequencies(int& length) const
    {
        // select the first measurement if exist. There should be no different
        // sampling intervals between measurement if more exist.
        shared_ptr<ProcessingInterface> pt = getProcess(processId_, 0); 
        if (pt.get() == 0)
            return NULL;
        
        return pt->getFrequencies(length);
    }

    const complex<double>* Root::getModes(double frequency, unsigned int measurementNumber, int& nchannels, int& nsvd) const 
    {
        shared_ptr<ProcessingInterface> proc = getProcess(processId_, measurementNumber);            
        if (proc.get() == 0)
            return NULL;
    
        int length(0);
        const double* frequencies = proc->getFrequencies(length);         
        if (frequencies == 0)
            return NULL;
        
        int index = Oi::find_nearest(frequencies, frequencies+length, frequency);
        arma::cx_mat& modes = proc->getModes(index);
        
        if (modes.is_empty())
            return NULL;
    
        // apply directions for every channel 
        shared_ptr<FileFormatInterface> format = getFileFormat(measurementNumber);      
        if (format.get() == 0)
            return NULL;
        
        const vector<ChannelInfo>* channelInfo = format->getChannelInfo(); 
        assert(channelInfo != 0);
        assert(channelInfo->size() == modes.n_rows);
        
        nchannels = (int)modes.n_rows;
        nsvd = (int)modes.n_cols;
        
        for (int r = 0; r < nchannels; ++r)
        {
            for ( int c = 0; c < nsvd; ++c)
                modes(r,c) = modes(r,c) * double(channelInfo->at(r).directionValue());
        }
        
        return modes.memptr();
    }
   
    shared_ptr<FileFormatInterface> Root::getFileFormat(unsigned int measurementNumber) const 
    {
        if (fileFormatList_.empty())
            return shared_ptr<FileFormatInterface>();
        
        unsigned int numberOfMeasurements(0);
        vector< shared_ptr<FileFormatInterface> >::const_iterator it;

        /*
         *it = std::find_if(fileFormatList_.begin(), 
         *              fileFormatList_.end(), 
         *              [&numberOfMeasurements, measurementNumber](shared_ptr<FileFormatInterface> format)->bool
         *{
         *    if (format->existChannels())
         *        ++numberOfMeasurements;
         *    if (numberOfMeasurements == (measurementNumber+1))
         *        return true;
         *    
         *    return false;
         *});
         *
         *if (it != fileFormatList_.end())
         *    return *it;
         */

        foreach(shared_ptr<FileFormatInterface> format, fileFormatList_)
        {
            if (format->existChannels())
                ++numberOfMeasurements;
            if (numberOfMeasurements == (measurementNumber+1))
                return format;
        }
        
        return shared_ptr<FileFormatInterface>();
    }

    shared_ptr<ProcessingInterface> Root::getProcess(int processId, unsigned int measurementNumber) const 
    {
        if (procList_.empty())
            return shared_ptr<ProcessingInterface>();
        
        /*
         *int numberOfMeasurements(0);
         *foreach(shared_ptr<ProcessingInterface> proc, procList_)
         *{
         *    if (proc->getProcessId() == processId)
         *        ++numberOfMeasurements;
         *    if (numberOfMeasurements == (measurementNumber+1))
         *        return proc;
         *}
         */
        
        vector< shared_ptr<ProcessingInterface> >::const_iterator proc_it;
        proc_it = find_if(procList_.begin(),
                          procList_.end(),
                          bind(std::equal_to<int>(),
                          bind(bind(&ProcessingInterface::getProcessId, _1),
                          _1),
                          processId));
        
        if (proc_it != procList_.end() && std::distance(proc_it, procList_.end()) > measurementNumber)
        {
             std::advance(proc_it, measurementNumber);     
             return *proc_it;
        }

        return shared_ptr<ProcessingInterface>();
    }


} // namespace Oi
