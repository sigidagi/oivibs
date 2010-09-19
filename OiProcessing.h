#ifndef _OIPROCESSING_H
#define _OIPROCESSING_H

#include <memory>

using std::auto_ptr;

namespace Oi 
{
    class StorageInterface;

    class ProcessingInterface
    {
        public:
            ProcessingInterface(StorageInterface* owner);
            virtual ~ProcessingInterface();

        public:
            static auto_ptr<ProcessingInterface> createProcess(StorageInterface* owner, int processName);
            virtual bool start() = 0;

        protected:
            StorageInterface* storage_;
    };

} // namespace Oi

#endif
