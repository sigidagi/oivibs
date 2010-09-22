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
//        Company:  sigidagis
// 
// =====================================================================================

#ifndef _OIUTIL_H
#define _OIUTIL_H

#include <string>
#include <boost/shared_ptr.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using std::string;



namespace Oi
{

/*
 *      template <class T>   
 *      inline typename std::tr1::remove_reference<T>::type&& move(T&& t)  
 *      {  
 *        return t;  
 *      } 
 *
 *      template<class T>
 *      inline void swap(T& a, T& b) 
 *      { 
 *          T dum(move(a)); 
 *          a = move(b); 
 *          b = move(dum); 
 *      }
 */

    enum ProcessName
    {
        FDD, // frequency domain decomposition
        SSI  // stochastic subspace identification
             // other posisible methods..
    };  


    template<class T>
    inline void swap(T& a, T& b) 
    { 
        T dum = a; 
        a = b; 
        b = dum; 
    }

    void free2D(double**, int);
    
    string stripToBaseName(const string& pathToFile);
    string stripToFileName(const string& pathToFile);
    string stripToExtension(const string& pathToFile);
    string stripToPath(const string& pathToFile);

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

}

#endif
