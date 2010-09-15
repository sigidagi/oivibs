#include "config.hpp"
#include "OiLocalStorage.h"

namespace Oi {

    LocalStorage* LocalStorage::instance_ = 0;

    /*
     *LocalStorage& LocalStorage::Instance()
     *{
     *    static LocalStorage thisLocalStorage;
     *    return thisLocalStorage;
     *}
     */


    LocalStorage* LocalStorage::Instance()
    {
        if (!instance_)
            instance_ = new LocalStorage;
        
        return instance_;
    }

    bool LocalStorage::init(const string& name, int processName)
    {
        return false;
    }

    bool LocalStorage::connect(const string& name)
    {
        return false;
    }

    double** LocalStorage::getNodes(int& size)
    {
        return NULL; 
    }

    double** LocalStorage::getLines(int& size)
    {
        return NULL;
    }

    double** LocalStorage::getSurfaces(int& size)
    {
        return NULL;
    }

} // namespace Oi
