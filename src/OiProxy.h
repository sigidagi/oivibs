// =====================================================================================
// 
//       Filename:  OiProxy.h
// 
//    Description:  Proxy class for object decoupling
// 
//        Version:  1.0
//        Created:  2010-09-06 16:26:16
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

#ifndef _OIPROXY_H
#define _OIPROXY_H

#include    <string>
#include	<complex>

using std::string;
using std::complex;

#define DLL __atribute__(dllexport)

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

namespace Oi {

    class ProxyInterface
    {
        public:
            virtual bool init(int argc, const char** fileList, int processName = 0) = 0;
            virtual bool connect(const string& name) = 0;
            
            virtual const double* getNodes(int&, int& ) const = 0;
            virtual const unsigned int* getLines(int&, int& ) const = 0;
            virtual const unsigned int* getSurfaces(int&, int& ) const = 0;
           
            virtual int getNumberOfMeasurements() const = 0;
            virtual int getNumberOfChannels(unsigned int measurementNumber) const = 0;
            virtual const double* getSpectralDensity(unsigned int measurementNumber, int& nsamples, int& nchannels) const = 0;

            virtual const double* getSingularValues(unsigned int mesurementNumber, int& nrows, int& ncols) const = 0; 
            virtual const double* getFrequencies(int& length) const = 0;
            virtual const complex<double>* getModes(double frequency, unsigned int measurementNumber, int& nchannels, int& nsvd) const = 0; 

            virtual bool selectProcess(int processId) = 0;
            virtual bool applyProcess(int processId) = 0; 

    };
} // namespace Oi


namespace Oi {

    class Proxy : public ProxyInterface
    {
        public:
            Proxy(); 
            ~Proxy ();

        // Interfaces
        public:
            bool init(int argc, const char** fileList, int processName = 0);
            bool connect(const string& name);

            const double* getNodes(int& nrows, int& ncols) const;
            const unsigned int* getLines(int& nrows, int& ncols) const;
            const unsigned int* getSurfaces(int& nrows, int& ncols) const;

            int getNumberOfMeasurements() const;
            int getNumberOfChannels(unsigned int measurementNumber) const;
            const double* getSpectralDensity(unsigned int measurementNumber, int& nsamples, int& nchannels) const;

            const double* getSingularValues(unsigned int mesurementNumber, int& nrows, int& ncols) const; 
            const double* getFrequencies(int& lenght) const;
            const complex<double>* getModes(double frequency, unsigned int measurementNumber, int& nchannels, int& nsvd) const; 

            bool selectProcess(int processId);
            bool applyProcess(int processId); 

        private:
            ProxyInterface* impl_; 
    };

} // namespace Oi

#endif
