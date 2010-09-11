
#include "OiUniversalFileFormat.h"
#include "OiAsciiFileFormat.h"
#include "OiFileFormat.h"
#include "OiUtil.h"

namespace Oi {

FileFormatInterface* FileFormatInterface::createFileFormat(const string& file)
{
        string extension = Oi::stripToExtension(file);

        if (extension == "uff")
            return new UniversalFileFormat;
        else if (extension == "txt")
            return new AsciiFileFormat;
        else
            return 0;
}

} // namspace Oi
