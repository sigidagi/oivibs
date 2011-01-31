// =====================================================================================
// 
//       Filename:  OiUFF.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-01-18 12:57:59
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


#ifndef  OIUFF_INC
#define  OIUFF_INC


#include    "OiUtil.h"	
#include	<map>
#include    <string>
#include	<vector>
#include	<algorithm>
#include	<boost/bind.hpp>

using std::string;

namespace Oi {

class UFF
{
    public:
        typedef UFF uff_type;

        virtual const int number() const = 0;
        virtual void setParameters(const string& file, int position, int nlines) = 0;
        virtual void parse() = 0; 

        virtual const void* getData(size_t& size) = 0;
};

template<class K>
class UFactory
{
    private:
        typedef UFF* (*createObjectFunc)();

        std::map<K, createObjectFunc> objectCreator;
        std::map<K, string> categoryList;

        template<typename S>
        static UFF* createObject()
        {
            return new S();
        }

    public:
       
        template<typename S>
        void registerClass(K id, const string& category)
        {
            if (objectCreator.find(id) != objectCreator.end())
            {
                // class already registered and exist
                // error handling here
                return;
            }
            objectCreator.insert( std::make_pair<K, createObjectFunc>(id, &createObject<S>) );
            categoryList.insert( std::make_pair<K, string>(id, category) );
        }
        
        void getRegistrationKeys( std::vector<int>& keys)
        {
            keys.clear();
            typename std::map<K, createObjectFunc>::iterator iter;
            for (iter = objectCreator.begin(); iter != objectCreator.end(); ++iter)
            {
                keys.push_back( (*iter).first );
            }
        }
       
        void selectKeysByCategory( std::vector<int>& keys, const string& category)
        {
            keys.clear();
            Oi::transform_if(categoryList.begin(),
                             categoryList.end(),
                             back_inserter(keys),
                             boost::bind(Oi::is_value_equal<int, string>, _1, category),
                             Oi::make_select1st(categoryList));
        }

        bool hasClass(K id)
        {
            return (objectCreator.find(id) != objectCreator.end() );
        }

        UFF* createObject(K id)
        {
            typename std::map<K, createObjectFunc>::iterator iter = objectCreator.find(id);
            if (iter == objectCreator.end())
                return NULL;

            return ((*iter).second)();
        }
};

} // namespace Oi

#endif   // ----- #ifndef OIUFF_INC  -----
