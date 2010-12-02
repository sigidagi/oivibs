#ifndef _OIPROCESSING_H
#define _OIPROCESSING_H


#include	"OiPersist.h"
#include    <boost/shared_ptr.hpp>

using boost::shared_ptr;

namespace Oi 
{
    class Root;

    class ProcessingInterface : public PersistInterface
    {
        public:
            ProcessingInterface(Root* owner);
            virtual ~ProcessingInterface();

        public:
            static shared_ptr<ProcessingInterface> createProcess(Root* owner, int processName);
            virtual bool start() = 0;

            // PersistInterface
            void save();
            void load();

        protected:
            Root* root_;
    };

} // namespace Oi

#endif
