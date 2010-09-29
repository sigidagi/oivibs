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
    private:
 
    // StorageInterface
    public: 
          void save(const string& name);
          void load(const string& name);

};


} // namespace Oi


#endif
