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
#include <armadillo>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


using std::string;
using std::pair;



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
    
    struct Process
    {
        enum Name 
        {
            FDD, // frequency domain decomposition
            SSI  // stochastic subspace identification
                 // other posisible methods..
        };  
    };


    void free2D(double**, int);
    
    string stripToBaseName(const string& pathToFile);
    string stripToFileName(const string& pathToFile);
    string stripToExtension(const string& pathToFile);
    string stripToPath(const string& pathToFile);

    

    /*
     *template<typename eT>
     *string matType(const arma::Mat<eT>& m)
     *{
     *    if (arma::is_float<eT>::value == true)
     *        return string("FLOAT");
     *    else if (arma::is_double<eT>::value == true)
     *        return string("DUOBLE");
     *    else if (arma::is_integer<eT>::value == true)
     *        return string("INT");
     *    else
     *        return string("");
     *    
     *}
     */

    template<typename T>
    T** allocate2D(arma::Mat<T> a)
    {
        if (a.n_elem == 0 || a.n_rows == 0) 
            return NULL;

        T** array = new T*[a.n_rows];
        for (size_t n = 0; n < a.n_rows; ++n)
            array[n] = new T[a.n_cols];

        // assign values
        for (size_t i = 0; i < a.n_rows; ++i)
        {
            for (size_t j = 0; j < a.n_cols; ++j)
                array[i][j] = a(i,j);
        }
        
        return array;
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
