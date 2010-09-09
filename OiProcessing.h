#ifndef _OIPROCESSING_H
#define _OILPROCESSING_H

#include "OiFormat.h"
#include <armadillo>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace arma;

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

    class OiProcessing
    {
        public:
            void createPSD(cube& psd, mat& chunk);
            bool start(OiFormat* pFormat);

        private:
            void detrend( Mat<double>& x, int p = 1);
            colvec hamming(int m);
            // pseudo inverse using SVD
            void inverse( Mat<double>& x );

        private:
            cube m_Pxx;
            mat m_SVD;
            colvec m_Freq;
    };


} // namespace Oi

/*
 *#ifdef __cplusplus
 *extern "C"
 *{
 *#endif
 *
 *namespace Oi
 *{
 *
 *} // namespace Oi
 *
 *#ifdef __cplusplus
 *}
 *#endif
 */


#endif
