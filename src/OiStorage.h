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

#define TO_STREAM(stream,variable) (stream) <<#variable

namespace Oi
{
    template<typename T>
    void pushToStream(std::stringstream& ss, const string& fileName, const string& varName, const T& var)
    {
       ss << fileName << " ";
       ss << varName << " "; 
       ss << 1 << " " << 1 << " ";
       ss << var;
    }

    template<typename T>
    void pushToStream(std::stringstream& ss, const string& fileName, const string& varName, const arma::Mat<T>& var)
    {
       ss << fileName << " ";
       ss << varName << " ";
       arma::mat A = arma::conv_to<arma::mat>::from(var); 
       ss << A.n_rows << " " << A.n_cols << " ";
       ss << A;
    }

  
    class StorageInterface
    {
        public:
            StorageInterface(); 
            virtual ~StorageInterface();
        
        // Interfaces
        public:
            virtual bool init(const string& repoName) = 0;
            virtual bool existRepository(const string& name) = 0;
           
            virtual void write(std::stringstream& ss) = 0;
            virtual void read(std::stringstream& ss) = 0;
            

    };

} // namespace Oi

#endif 
