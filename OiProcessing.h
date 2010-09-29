#ifndef _OIPROCESSING_H
#define _OIPROCESSING_H


#include	"OiPersist.h"
#include    <memory>

using std::auto_ptr;

namespace Oi 
{
    class Root;

    class ProcessingInterface : public PersistInterface
    {
        public:
            ProcessingInterface(Root* owner);
            virtual ~ProcessingInterface();

        public:
            static auto_ptr<ProcessingInterface> createProcess(Root* owner, int processName);
            virtual bool start() = 0;

            void save(const string& name);
            void load(const string& name);

        protected:
            Root* root_;
    };

} // namespace Oi

#endif
