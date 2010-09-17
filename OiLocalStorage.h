#ifndef _OIGEOMETRY_H
#define _OIGEOMETRY_H

#include <vector>
#include <string>

#include "OiProxy.h"
#include "OiStorage.h"

using std::vector;
using std::string;


namespace Oi {

class LocalStorage : public ProxyInterface, public StorageInterface
{
    private:
        LocalStorage(){};
        LocalStorage(LocalStorage const&){};
        LocalStorage& operator=(LocalStorage const&){ return *this;};

        static LocalStorage* instance_;
    
    // ProxyInterface
    public:
        bool init(const string& name, int processName = 0); 
        bool connect(const string& name);
        double** getNodes(int& size);
        double** getLines(int& size);
        double** getSurfaces(int& size);
    
    // StorageInterface
    public: 
        void saveNodes(const arma::mat& nodes);
        void saveLines(const arma::umat& lines);
        void saveSurfaces(const arma::umat& surfaces);
        void saveData(const arma::mat& data);

   
        LocalStorage* Instance(); 
};


} // namespace Oi


#endif
