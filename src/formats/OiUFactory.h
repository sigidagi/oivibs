// =====================================================================================
// 
//       Filename:  OiUFactory.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  05/13/2011 10:46:43 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Sigitas Dagilis (), sigidagi@gmail.com
//
//     This file is part of the OiVibrations C++ library.
//     It is provided without any warranty of fitness
//     for any purpose. You can redistribute this file
//     and/or modify it under the terms of the GNU
//     General Public License (GPL) as published
//     by the Free Software Foundation, either version 3
//     of the License or (at your option) any later version.
//     (see http://www.opensource.org/licenses for more info)
// 
// =====================================================================================

#ifndef  OIUFACTORY_INC
#define  OIUFACTORY_INC

#include    "OiUtil.h"	
#include    "OiUFFAll.h"	
#include	<map>
#include	<list>
#include    <string>
#include	<algorithm>
#include	<boost/any.hpp>
#include	<boost/bind.hpp>
#include	<boost/function.hpp>

using std::map;
using std::string;
using boost::function;

namespace Oi {

template<class K>
class UFactory
{
    private:
        function<UFF* ()> createObjectFunc;

        map<K, function<UFF* ()> > objectCreator;

        template<typename S>
        static UFF* createObject()
        {
            return new S();
        }
    
    public:
        template<typename S>
        void registerClass(K id)
        {
            if (objectCreator.find(id) != objectCreator.end())
            {
                // class already registered and exist
                // error handling here
                return;
            }

            createObjectFunc = &createObject<S>;
            std::pair<K, function<UFF* ()> > object(id, createObjectFunc);
            objectCreator.insert( object );
        }
        
        void getRegistrationKeys( std::vector<int>& keys)
        {
            keys.clear();
            typename map<K, function<UFF* ()> >::iterator iter;
            for (iter = objectCreator.begin(); iter != objectCreator.end(); ++iter)
            {
                keys.push_back( (*iter).first );
            }
        }
       
        bool hasClass(K id)
        {
            return (objectCreator.find(id) != objectCreator.end() );
        }

        UFF* createObject(K id)
        {
            typename std::map<K, function<UFF* ()> >::iterator iter = objectCreator.find(id);
            if (iter == objectCreator.end())
                return NULL;

            return ((*iter).second)();
        }
        
        void registerAllFormats()
        {
            // Universal Dataset number is ID for registration of supported Universal Formats.
            this->registerClass<UFF15>(15);
            this->registerClass<UFF10015>(10015);
            
            this->registerClass<UFF82>(82);
            this->registerClass<UFF10082>(10082);

            this->registerClass<UFF2412>(2412);
            this->registerClass<UFF10012>(10012);
            
            this->registerClass<UFF58>(58);
            this->registerClass<UFF10058>(10058);

            // .... add more below ....
        }

};



} // namespace Oi

#endif   // ----- #ifndef OIUFACTORY_INC  -----
