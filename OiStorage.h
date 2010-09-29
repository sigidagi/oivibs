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
//        Company:  sigidagis
// 
// =====================================================================================

#ifndef _OISTORAGE_H
#define _OISTORAGE_H

#include	<string>

using std::string;

namespace Oi
{
    class StorageInterface
    {
        public:
            StorageInterface(); 
            virtual ~StorageInterface();
        
        // Interfaces
        public:
             virtual bool init(const string& repoName) = 0;

             template<class T>
             void write(const string& repoName, const string& name, const T& variable){}
             template<class T>
             void read(const string& repoName, const string& name, T& variable){}
    };

} // namespace Oi

#endif 
