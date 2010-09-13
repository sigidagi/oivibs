#ifndef _OIPROCESSING_H
#define _OIPROCESSING_H

namespace Oi 
{
    class FileFormatInterface;
    
    class ProcessingInterface
    {
        public:
            virtual ~ProcessingInterface(){};

        public:
            static ProcessingInterface* createProcess(int processName);
            virtual bool start(FileFormatInterface* fileFormat) = 0;
    };

} // namespace Oi

#endif
