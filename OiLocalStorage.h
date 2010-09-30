#ifndef _OIGEOMETRY_H
#define _OIGEOMETRY_H

#include <vector>
#include <string>

#include "OiProxy.h"
#include "OiStorage.h"

using std::vector;
using std::string;


namespace Oi {

    class LocalStorage : public StorageInterface
    {
        public:
            LocalStorage(){}
     
        // StorageInterface
        public: 
             bool init(const string& repoName);

    };


} // namespace Oi


#endif
