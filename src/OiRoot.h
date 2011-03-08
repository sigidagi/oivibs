// =====================================================================================
// 
//       Filename:  OiRoot.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-29 11:02:16
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


#ifndef  _OIROOT_H
#define  _OIROOT_H

#include	"OiProxy.h"
#include    <boost/shared_ptr.hpp>
#include    <vector>

using boost::shared_ptr;
using std::vector;

namespace Oi
{
    // forward declaration.
    class FileFormatInterface;
    class ProcessingInterface;

    class Root : public ProxyInterface
    {
        private:
                Root();
                ~Root();
                Root(Root const&);
                Root& operator=(Root const&);

                static Root* instance_;
                int processId_;

                vector<string> fileList_;
                vector< shared_ptr<FileFormatInterface> >fileFormatList_;
                vector< shared_ptr<ProcessingInterface> > procList_;
            
            // ProxyInterface 
            public:
                bool init(int argc, const char** fileList, int processName = 0);
                bool connect(const string& repoName);
                
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

            public:
                static Root* Instance();
                
                shared_ptr<FileFormatInterface> getFileFormat(unsigned int measurementNumber) const;
                shared_ptr<ProcessingInterface> getProcess(int processId, unsigned int measurementNumber) const; 

    };

} // namespace Oi


#endif   // ----- #ifndef OIROOT_INC  -----


