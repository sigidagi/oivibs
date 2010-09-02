#ifndef _SIGNALPROCESSING_H
#define _SIGNALPROCESSING_H

#include <armadillo>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace arma;
class UniversalFormat;

#define DLL __atribute__(dllexport)


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

      template<class T>
      inline void swap(T& a, T& b) 
      { 
          T dum = a; 
          a = b; 
          b = dum; 
      }


}

#ifdef __cplusplus
extern "C"
{
#endif

namespace Oi
{
        // pseudo inverse using SVD
        void inverse( Mat<double>& x );
        
        void detrend( Mat<double>& x, int p = 1);
        
        colvec hamming(int m);
        
/*
 *        void fftw(Mat<double>& x); 
 *        
 *        void ffta(double* px, unsigned long nelements);
 *
 *        void four1(double* data, unsigned long nn);
 */

} // namespace Oi

#ifdef __cplusplus
}
#endif


#endif
