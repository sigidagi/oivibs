/***************************************************************************
    Copyright (C) 2006 by Volodymyr Myrnyy (Vladimir Mirnyi)
 ***************************************************************************
 Permission to use, copy, modify, distribute and sell this software for any
 purpose is hereby granted without fee, provided that the above copyright
 notice appear in all copies and that both that copyright notice and this
 permission notice appear in supporting documentation.
 ***************************************************************************

 Generic simple and efficient Fast Fourier Transform (FFT) implementation
 using template metaprogramming

 ***************************************************************************/


#ifndef  GFFT_INC
#define  GFFT_INC

#include <iostream>
#include <iomanip>
#include <cmath>
#include "loki/Typelist.h"
#include "loki/Factory.h"

using namespace std;

namespace Oi {

    ////// template class SinCosSeries
    // common series to compile-time calculation
    // of sine and cosine functions

    template<unsigned M, unsigned N, unsigned B, unsigned A>
    struct SinCosSeries {
       static double value() {
          return 1-(A*M_PI/B)*(A*M_PI/B)/M/(M+1)
                   *SinCosSeries<M+2,N,B,A>::value();
       }
    };

    template<unsigned N, unsigned B, unsigned A>
    struct SinCosSeries<N,N,B,A> {
       static double value() { return 1.; }
    };

    ////// template class Sin
    // compile-time calculation of sin(A*M_PI/B) function

    template<unsigned B, unsigned A, typename T=double>
    struct Sin;

    template<unsigned B, unsigned A>
    struct Sin<B,A,float> {
       static float value() {
          return (A*M_PI/B)*SinCosSeries<2,24,B,A>::value();
       }
    };
    template<unsigned B, unsigned A>
    struct Sin<B,A,double> {
       static double value() {
          return (A*M_PI/B)*SinCosSeries<2,34,B,A>::value();
       }
    };

    ////// template class Cos
    // compile-time calculation of cos(A*M_PI/B) function

    template<unsigned B, unsigned A, typename T=double>
    struct Cos;

    template<unsigned B, unsigned A>
    struct Cos<B,A,float> {
       static float value() {
          return SinCosSeries<1,23,B,A>::value();
       }
    };
    template<unsigned B, unsigned A>
    struct Cos<B,A,double> {
       static double value() {
          return SinCosSeries<1,33,B,A>::value();
       }
    };


    ////// template class DanielsonLanczos
    // Danielson-Lanczos section of the FFT

    template<unsigned N, typename T=double>
    class DanielsonLanczos {
       DanielsonLanczos<N/2,T> next;
    public:
       void apply(T* data) {
          next.apply(data);
          next.apply(data+N);

          T wtemp,tempr,tempi,wr,wi,wpr,wpi;
    //    Change dynamic calculation to the static one
    //      wtemp = sin(M_PI/N);
          wtemp = -Sin<N,1,T>::value();
          wpr = -2.0*wtemp*wtemp;
    //      wpi = -sin(2*M_PI/N);
          wpi = -Sin<N,2,T>::value();
          wr = 1.0;
          wi = 0.0;
          for (unsigned i=0; i<N; i+=2) {
            tempr = data[i+N]*wr - data[i+N+1]*wi;
            tempi = data[i+N]*wi + data[i+N+1]*wr;
            data[i+N] = data[i]-tempr;
            data[i+N+1] = data[i+1]-tempi;
            data[i] += tempr;
            data[i+1] += tempi;

            wtemp = wr;
            wr += wr*wpr - wi*wpi;
            wi += wi*wpr + wtemp*wpi;
          }
       }
    };

    template<typename T>
    class DanielsonLanczos<4,T> {
    public:
       void apply(T* data) {
          T tr = data[2];
          T ti = data[3];
          data[2] = data[0]-tr;
          data[3] = data[1]-ti;
          data[0] += tr;
          data[1] += ti;
          tr = data[6];
          ti = data[7];
          data[6] = data[5]-ti;
          data[7] = tr-data[4];
          data[4] += tr;
          data[5] += ti;

          tr = data[4];
          ti = data[5];
          data[4] = data[0]-tr;
          data[5] = data[1]-ti;
          data[0] += tr;
          data[1] += ti;
          tr = data[6];
          ti = data[7];
          data[6] = data[2]-tr;
          data[7] = data[3]-ti;
          data[2] += tr;
          data[3] += ti;
       }
    };

    template<typename T>
    class DanielsonLanczos<2,T> {
    public:
       void apply(T* data) {
          T tr = data[2];
          T ti = data[3];
          data[2] = data[0]-tr;
          data[3] = data[1]-ti;
          data[0] += tr;
          data[1] += ti;
       }
    };

    ////// template class AbstractFFT
    // abstract base class to build an object factory

    template<typename T>
    class AbstractFFT {
    public:
       virtual void fft(T*) = 0;
    };

    ////// template class EmptyFFT
    // abstract empty base class to pass instead of AbstractFFT
    // (if object factory is not needed)
    // and avoid a virtual function call penalty

    class EmptyFFT { };


    ////// template class GFFT
    // generic fast Fourier transform main class

    template<unsigned P, typename T=double,
    class FactoryPolicy=EmptyFFT>
    class GFFT:public FactoryPolicy {
       enum { N = 1<<P };
       DanielsonLanczos<N,T> recursion;
       void scramble(T* data) {
         int i,m,j=1;
         for (i=1; i<2*N; i+=2) {
            if (j>i) {
                swap(data[j-1], data[i-1]);
                swap(data[j], data[i]);
            }
            m = N;
            while (m>=2 && j>m) {
                j -= m;
                m >>= 1;
            }
            j += m;
         }
       }
    public:
       enum { id = P };
       static FactoryPolicy* Create() {
          return new GFFT<P,T,FactoryPolicy>();
       }
       void fft(T* data) {
          scramble(data);
          recursion.apply(data);
       }
    };

    ////// template class FactoryInit
    // object factory initialization class
    // register all the classes from TList in
    // object factory Fact

    template<class TList>
    struct FactoryInit;

    template<class H, class T>
    struct FactoryInit<Loki::Typelist<H,T> > {
       template<class Fact>
       static void apply(Fact& f) {
          f.Register(H::id,H::Create);
          FactoryInit<T>::apply(f);
       }
    };

    template<>
    struct FactoryInit<Loki::NullType> {
       template<class Fact>
       static void apply(Fact&) { }
    };

    ////// template class GFFTList
    // create a Typelist of given FFT classes to pass it
    // to the initialization class FactoryInit

    template<
    template<unsigned,class,class> class FFT,
    unsigned Begin, unsigned End,
    typename T=double,
    class FactoryPolicy=AbstractFFT<T> >
    struct GFFTList {
       typedef Loki::Typelist<FFT<Begin,T,FactoryPolicy>,
               typename GFFTList<FFT,Begin+1,End,T,
                        FactoryPolicy>::Result> Result;
    };

    template<
    template<unsigned,class,class> class FFT,
    unsigned End, typename T, class FactoryPolicy>
    struct GFFTList<FFT,End,End,T,FactoryPolicy> {
       typedef Loki::NullType Result;
    };


    typedef double Tp;
    

    // added by Sigitas Dagilis
    // Wraper class GiFFT
    //
    template<class In, class Out, class BinaryOperator>
    Out transform_pairs(In first, In last, Out res, BinaryOperator op)
    {
        In prev;
        while (first != last)
        {
           prev = first++; 
           *res++ = op(*prev, *first); 
           ++first;
        }
        return res;
    }

    class GiFFT
    {
        private:
            unsigned int dataLength_;       
            AbstractFFT<double>* gfft_;

            void init()
            {
                // initialization of the object factory
                Loki::Factory<AbstractFFT<double>,unsigned int> gfft_factory;
                FactoryInit<GFFTList<GFFT,1,27>::Result>::apply(gfft_factory);

                // runtime definition of the data length
                // numberOfSamples = 1 << spectraLength_, i.e. 2^dataLength_ 

                // create an instance of the GFFT
                unsigned int power2 = std::log2(dataLength_);
                gfft_ = gfft_factory.CreateObject(power2-1);
            }

        public:
            GiFFT() : dataLength_(2048) { init(); } 
            GiFFT(unsigned int n) : dataLength_(n) { init(); }
            
            void setDataLength(unsigned int n)
            {
                dataLength_ = n;
            }

            void exec(double* in, complex<double>* out)
            {
                gfft_->fft(in);
                Oi::transform_pairs(in,in+dataLength_, out, [](double v1, double v2)->complex<double>
                {
                     return complex<double>(v1,v2); 
                });
                    
            }
    };
   

} // namespace Oi

#endif   // ----- #ifndef GFFT_INC  -----
