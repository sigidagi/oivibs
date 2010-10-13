#include "OiRoot.h"
#include "OiUniversalFileFormat.h"
#include "OiAsciiFileFormat.h"
#include "OiFileFormat.h"
#include "OiUtil.h"

namespace Oi 
{
    FileFormatInterface::FileFormatInterface(Root* owner) : root_(owner)
    {

    }

    shared_ptr<FileFormatInterface> FileFormatInterface::createFileFormat(Root* owner, const string& file)
    {
            string extension = Oi::stripToExtension(file);

            if (extension == "uff")
            {
                return shared_ptr<FileFormatInterface>(new UniversalFileFormat(owner));
            }
            else if (extension == "txt")
            {
                return shared_ptr<FileFormatInterface>(new AsciiFileFormat(owner));
            }
            else
            {
                shared_ptr<FileFormatInterface> pt;
                return pt;
            }
    }

    double FileFormatInterface::getSamplingInterval()
    {
        return samplingInterval_;
    }

    int FileFormatInterface::getNumberOfSamples()
    {
        return numberOfSamples_;
    }

    void FileFormatInterface::save()
    {
        std::stringstream ss;
        string fileName = file_;
        if (existNodes())
        {
            pushToStream(ss, fileName, "nodes_", nodes_);
            root_->getStorage()->write(ss);                        
        }
        if (existLines())
        {
            ss.str("");
            ss.clear();
            pushToStream(ss, file_, "lines_", lines_);
            root_->getStorage()->write(ss);
        }
        if (existSurfaces())
        {
            ss.str("");
            ss.clear();
            pushToStream(ss, file_, "surfaces_", surfaces_);
            root_->getStorage()->write(ss);
        }
    }

    void FileFormatInterface::load()
    {

    }
    
} // namspace Oi
