// =====================================================================================
// 
//       Filename:  OiUniversalFileFormat.h
// 
//    Description:  Utilities functions
// 
//        Version:  1.0
//        Created:  2010-09-06 21::14
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

#ifndef _UNIVERSALFORMAT_H
#define _UNIVERSALFORMAT_H

#include "OiFileFormat.h"
#include <vector>
#include <string>
#include <armadillo>
#include <boost/shared_ptr.hpp>

using boost::shared_ptr;
using std::vector;
using std::string;

namespace Oi {

    // forward declaration.
    class Root;

    class UniversalFileFormat : public FileFormatInterface
    {
        // pivate data
    private:
        class Info 
        {
            public:
                Info(FileFormatInterface* self, const string& file) : self_(self), file_(file) {}
                virtual void parse() = 0;
                void setPosition(int pos) { position_ = pos; }
            protected:
                FileFormatInterface* self_;
                string file_;
                int position_;
        };

        class NodeInfo : public Info
        {
            public:
                NodeInfo(FileFormatInterface* self, const string& file);
                void parse();
        };

        class LineInfo : public Info
        {
            public:
                LineInfo(FileFormatInterface* self, const string& file);
                void parse();
        };

        class SurfaceInfo : public Info
        {
            public:
                SurfaceInfo(FileFormatInterface* self, const string& file);
                void parse();
        };

        class RecordInfo: public Info
        {
            public:
                RecordInfo(FileFormatInterface* self, const string& file, int recordnumber);
                void parse();
                void parseHeader();
                double getSamplingInterval();
                int getNumberOfSamples();
            private:
                int recordnumber_;
                double samplingInterval_;
                int numberOfSamples_;
        };

        vector< shared_ptr<Info> > info_;

    private:
        template<typename T>
        bool existInfo()
        {
            vector< shared_ptr<Info> >::iterator it;
            for (it = info_.begin(); it != info_.end(); ++it)
            {
                if(boost::dynamic_pointer_cast<T>(*it))
                    return true;
            }
            return false;
        }
        
    public:
        UniversalFileFormat(Root* owner);
        ~UniversalFileFormat();

        // FileFormatInterface
        void parse(const string& file);

        bool existNodes();
        bool existLines();
        bool existSurfaces();
        bool existRecords();

        arma::mat& getNodes();
        arma::umat& getLines();
        arma::umat& getSurfaces();
        arma::mat& getRecords();

        /*
         *void save();
         *void load();
         */
    };

    
} // namespace Oi

#endif
