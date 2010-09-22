#include "OiStorage.h"
#include "OiUniversalFileFormat.h"
#include "OiAsciiFileFormat.h"
#include "OiFileFormat.h"
#include "OiUtil.h"

namespace Oi {

FileFormatInterface::FileFormatInterface(StorageInterface* storage) : storage_(storage)
{

}

auto_ptr<FileFormatInterface> FileFormatInterface::createFileFormat(StorageInterface* owner, const string& file)
{
        string extension = Oi::stripToExtension(file);

        if (extension == "uff")
        {
            return auto_ptr<FileFormatInterface>(new UniversalFileFormat(owner));
        }
        else if (extension == "txt")
        {
            return auto_ptr<FileFormatInterface>(new AsciiFileFormat(owner));
        }
        else
        {
            auto_ptr<FileFormatInterface> pt;
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

StorageInterface* FileFormatInterface::getStorage()
{
    return storage_;
}

void FileFormatInterface::setStorage(StorageInterface* storage)
{
    storage_ = storage;
}

} // namspace Oi
