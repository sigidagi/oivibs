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
#include	<list>
#include    <string>
#include	<vector>
#include	<fstream>
#include	<algorithm>
#include	<boost/any.hpp>
#include	<boost/bind.hpp>
#include	<boost/function.hpp>
#include	<boost/lexical_cast.hpp>
#include	<boost/numeric/conversion/cast.hpp>

using std::map;
using std::string;
using boost::function;
using boost::lexical_cast;
using boost::numeric_cast;
using boost::bad_lexical_cast;
using boost::bad_numeric_cast;


namespace Oi {

class UFF
{
    public:
        typedef UFF uff_type;

        virtual const int number() const = 0;
        virtual void setParameters(const string& file, int position, int nlines) = 0;
        virtual void parse() = 0; 

        virtual const void* getData(size_t& size) = 0;
        virtual boost::any getExtraData() 
        {
            return boost::any();
        }

    protected:
        void advanceLines(std::ifstream& fs, unsigned int numberOfLines)
        {
            string temp;
            for (unsigned int i = 0; i < numberOfLines; ++i)
                getline(fs, temp);
        }
};

template<class K>
class UFactory
{
    private:
        function<UFF* ()> createObjectFunc;

        map<K, function<UFF* ()> > objectCreator;
        map<K, string> categoryList;

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

            createObjectFunc = &createObject<S>;
            std::pair<K, function<UFF* ()> > object(id, createObjectFunc);
            objectCreator.insert( object );
                
            std::pair<K, string> cat(id, category);    
            categoryList.insert( cat );
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
       
        void selectKeysByCategory( std::vector<int>& keys, const string& category)
        {
            keys.clear();
            Oi::transform_if(categoryList.begin(),
                             categoryList.end(),
                             back_inserter(keys),
                             boost::bind(Oi::is_value_equal<int, string>, _1, category),
                             Oi::make_select1st(categoryList));
        }

        string selectCategory(K id)
        {
            typename std::map<K, string>::iterator it;
            it = categoryList.find(id);
            if (it != categoryList.end())
                return it->second;
            else
                return string();
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
};

} // namespace Oi

#endif   // ----- #ifndef OIUFF_INC  -----
