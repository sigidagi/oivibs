// =====================================================================================
// 
//       Filename:  OiAsciiFileFormat.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-09 10:31:18
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


#ifndef _OIASCIIFILEFORMAT_H
#define _OIASCIIFILEFORMAT_H

#include "OiFileFormat.h"
#include <armadillo>
#include <string>
#include <fstream>

using std::string;

namespace Oi {
    
    class Root;

    class AsciiFileFormat : public FileFormatInterface  
    {
        public:
            AsciiFileFormat(Root* owner, const string& file);
            ~AsciiFileFormat(){}

        public:
            void parse();
            
            bool existNodes() const;
            bool existLines() const;
            bool existSurfaces() const;
            bool existChannels() const;
            int getNumberOfChannels() const;

            const arma::mat& getChannels() const;
            double getSamplingInterval() const;
            string getFileName() const;

        private:
            Root* root_; 
            string file_;
            
            arma::mat channels_;

            std::ifstream fileStream_;
            bool existChannels_;
    };

} // namespace Oi

#endif
