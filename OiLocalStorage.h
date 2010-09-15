#ifndef _OIGEOMETRY_H
#define _OIGEOMETRY_H

#include <vector>
#include <string>

#include "OiProxy.h"

using std::vector;
using std::string;


namespace Oi {

class LocalStorage : public ProxyInterface
{
    private:
        LocalStorage(){};
        LocalStorage(LocalStorage const&){};
        LocalStorage& operator=(LocalStorage const&){ return *this;};

        static LocalStorage* instance_;
        
    public:
        bool init(const string& name, int processName = 0); 
        bool connect(const string& name);
        double** getNodes(int& size);
        double** getLines(int& size);
        double** getSurfaces(int& size);

        LocalStorage* Instance(); 
};


} // namespace Oi


#endif
