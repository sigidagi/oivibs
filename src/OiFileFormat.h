// =====================================================================================
// 
//       Filename:  OiFormat.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-09 13:30:50
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

#ifndef _OIFORMAT_H
#define _OIFORMAT_H


#include	"OiPersist.h"
#include    <armadillo>
#include    <string>
#include	<vector>
#include    <boost/shared_ptr.hpp>

using boost::shared_ptr;
using std::string;

namespace Oi {
    
    class Root;

    class FileFormatInterface : public PersistInterface
    {
        public:
            FileFormatInterface(Root* owner);
            virtual ~FileFormatInterface(){}

        public:
            static shared_ptr<FileFormatInterface> createFileFormat(Root* owner, const string& file);

            virtual void parse(const string& file) = 0;
            
            virtual bool existNodes() = 0;
            virtual bool existLines() = 0;
            virtual bool existSurfaces() = 0;
            virtual bool existRecords() = 0;

            virtual arma::mat& getNodes() = 0;
            virtual arma::umat& getLines() = 0;
            virtual arma::umat& getSurfaces() = 0;
            virtual arma::mat& getRecords() = 0;
            
            // PersistInterface
            void save();
            void load();

            double getSamplingInterval();
            int getNumberOfSamples();
            
        protected:
            Root* root_;
            
            string file_; 
        
            arma::mat nodes_;
            arma::umat lines_;
            arma::umat surfaces_;
            arma::mat records_;
            
            std::vector<string> variableNames; 

            double samplingInterval_;
            int numberOfSamples_;
    };

} // namespace Oi

#endif
