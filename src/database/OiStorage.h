// =====================================================================================
// 
//       Filename:  OiStorage.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-17 08:38:28
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

#ifndef _OISTORAGE_H
#define _OISTORAGE_H

#include    <armadillo>
#include    <sstream>
#include	<string>

using std::string;


namespace Oi
{
    template<typename T>
    struct SerializableObject
    {
        string& fileName_;
        string& variableName_;
        T& variable_;

        SerializableObject(string& fileName, string& variableName, T& variable) :  
            fileName_(fileName),
            variableName_(variableName),
            variable_(variable)
        {
        }

    };

/*
 *    template<typename Derived>
 *    struct StorageInterface
 *    {
 *         bool init(const string& name)
 *         {
 *             return static_cast<Derived*>(this)->init(name);
 *         }
 *         bool existRepository(const string& name)
 *         {
 *             return static_cast<Derived*>(this)->existRepository(name);
 *         }
 *
 *         template<typename T>
 *         void write(SerializableObject<T>& object)
 *         {
 *             return static_cast<Derived*>(this)->write(object);
 *         }
 *
 *         template<typename T>
 *         void read(SerializableObject<T>& object)
 *         {
 *             return static_cast<Derived*>(this)->read(object);
 *         }
 *    };
 */


/*
 *
 *    template<typename StorageInterface, typename T>
 *    void writeToStorage(StorageInterface& storage, SerializableObject<T>& object)
 *    {
 *         storage.write(object);    
 *    }
 *
 *    template<typename StorageInterface, typename T>
 *    void readFromStorage(StorageInterface& storage, SerializableObject<T>& object)
 *    {
 *         storage.read(object);    
 *    }
 *
 */

    class StorageInterface
    {
        public:
            StorageInterface(); 
            virtual ~StorageInterface();
        
        // Interfaces
        public:
            virtual bool init(const string& repoName) = 0;
            virtual bool existRepository(const string& name) = 0;
           
            template<typename T>
            void write(SerializableObject<T>& serData)
            {

            }
            template<typename T>
            void read(SerializableObject<T>& serData)
            {

            }

    };

} // namespace Oi

#endif 
