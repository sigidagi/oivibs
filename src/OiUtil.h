// =====================================================================================
// 
//       Filename:  OiUtil.h
// 
//    Description:  Utilities functions
// 
//        Version:  1.0
//        Created:  2010-09-07 11:36:18
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

#ifndef _OIUTIL_H
#define _OIUTIL_H

#include    <string>
#include    <boost/shared_ptr.hpp>
#include    <armadillo>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using std::string;
using std::pair;

namespace Oi
{
    
    struct Process
    {
        enum Name 
        {
            FDD, // frequency domain decomposition
            SSI  // stochastic subspace identification
                 // other posible methods..
        };  
    };

    
    string stripToBaseName(const string& pathToFile);
    string stripToFileName(const string& pathToFile);
    string stripToExtension(const string& pathToFile);
    string stripToPath(const string& pathToFile);
    

    // Array has to be sorted first. Returns index of nearest to the appled value. 
    template<class Iterator, class T>
    ptrdiff_t find_nearest(Iterator first, Iterator last, T value)
    {
        ptrdiff_t ret = 0;
        while (first != last)
        {
            if (*first > value)
                break;
            ++first;
            ++ret;
        }
        return ret;
    }
    
    template<class T> 
    struct sort_index
    {
        sort_index(const T arr) : arr_(arr) {}
        bool operator()(const size_t a, const size_t b) const
        { 
            return arr_[a] < arr_[b]; 
        }
        const T arr_;
    };
    

    template<typename In, typename Out, typename Pred>
    Out copy_if(In first, In last, Out res, Pred Pr)
    {
      while (first != last)
      {
        if (Pr(*first))
          *res++ = *first;
        ++first;
      }
      return res;
    }
    
    template<typename T>
    struct select1st
    {
        typename T::first_type operator()(const T& key) const
        {
            return key.first;
        }
    };

    template<typename T>
    select1st<typename T::value_type> make_select1st(const T& m)
    {
        return select1st<typename T::value_type>();
    }

    template<typename In, typename Out, typename Pred, typename UnaryOp>
    const Out transform_if(In first, const In last, Out res, const Pred pr, const UnaryOp uop)
    {
        while (first != last)
        {
            if (pr(*first))
                *res++ = uop(*first);
            ++first;
        }
        return res;
    }
    
    template<class K, class V>
    bool is_value_equal( std::pair<const K, V>& elem, const V& value)
    {
        return (elem.second == value);
    };


    template<typename In>
    bool equal_elements(In next, In last)
    {
        while (next != last-1)
        {
            if (*next != *(++next))
                return false;
        }
        return true;
    }
   
    template<class T>
    bool from_string(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&))
    {
        std::istringstream iss(s);
        return !(iss >> f >> t).fail();
    }
}

#endif
