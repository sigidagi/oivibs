#include "OiStorage.h"
#include "OiUniversalFileFormat.h"
#include "OiAsciiFileFormat.h"
#include "OiFileFormat.h"
#include "OiUtil.h"

namespace Oi {

FileFormatInterface::FileFormatInterface(StorageInterface* storage) : storage_(storage)
{

}

FileFormatInterface* FileFormatInterface::createFileFormat(StorageInterface* owner, const string& file)
{
        string extension = Oi::stripToExtension(file);

        if (extension == "uff")
            return new UniversalFileFormat(owner);
        else if (extension == "txt")
            return new AsciiFileFormat(owner);
        else
            return 0;
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
