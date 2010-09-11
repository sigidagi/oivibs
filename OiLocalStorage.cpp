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

    bool LocalStorage::init(const string& name)
    {
        return false;
    }

    bool LocalStorage::connect(const string& name)
    {
        return false;
    }

    void LocalStorage::getNodes(double** array, int& nnodes)
    {
     
    }

    void LocalStorage::getLines(double** array, int& nlines)
    {

    }

    void LocalStorage::getSurfaces(double** array, int& nsurfaces)
    {

    }

} // namespace Oi
