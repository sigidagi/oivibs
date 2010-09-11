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
        bool init(const string& name); 
        bool connect(const string& name);
        void getNodes(double** array, int& nnnodes);
        void getLines(double** array, int& nlines);
        void getSurfaces(double** array, int& nSurfaces);

        LocalStorage* Instance(); 
};


} // namespace Oi


#endif
