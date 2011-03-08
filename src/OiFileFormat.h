// =====================================================================================
// 
//       Filename:  OiFileFormat.h
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

#include	"OiChannelInfo.h"
#include    <string>
#include	<vector>
#include	<armadillo>
#include    <boost/shared_ptr.hpp>

using boost::shared_ptr;
using std::string;
using std::vector;

namespace Oi {
    
    class Root;

    class FileFormatInterface
    {
        public:
            virtual ~FileFormatInterface(){}

        public:
            static shared_ptr<FileFormatInterface> createFileFormat(Root* owner, const string& file);

            virtual void parse() = 0;
            
            virtual bool existNodes() const = 0;
            virtual bool existLines() const = 0;
            virtual bool existSurfaces() const = 0;
            virtual bool existChannels() const = 0;
           
            virtual const arma::mat& getChannels() const = 0;
            virtual double getSamplingInterval() const = 0;
            
            virtual int getNumberOfChannels() const = 0;
            virtual const double* getNodes(int& nrows, int& ncols) const;
            virtual const unsigned int* getLines(int& nrows, int& ncols) const;
            virtual const unsigned int* getSurfaces(int& nrows, int& ncols) const;
            virtual const vector<ChannelInfo>* getChannelInfo() const;

            virtual string getFileName() const = 0;

    };

/*
 *            friend class boost::serialization::access;
 *            template<typename Archive>
 *            void save(Archive& ar, unsigned int version)
 *            {
 *                int i; int itemp; double dtemp;
 *                // nodes
 *                ar & nodes_.n_rows;
 *                ar & nodes_.n_cols;
 *                for (i = 0; i < nodes_.n_elem; ++i)
 *                {
 *                    dtemp = nodes_(i); ar & dtemp;
 *                }
 *                // lines
 *                ar & lines_.n_rows;
 *                ar & lines_.n_cols;
 *                for (i = 0; i < lines_.n_elem; ++i)
 *                {
 *                    itemp = lines_(i); ar & itemp;
 *                }
 *                // surfaces
 *                ar & surfaces_.n_rows;
 *                ar & surfaces_.n_cols;
 *                for (i = 0; i < surfaces_.n_elem; ++i)
 *                {
 *                    itemp = surfaces_(i); ar & itemp;
 *                }
 *                // records
 *                ar & records_.n_rows;
 *                ar & records_.n_cols;
 *                for (i = 0; i < records_.n_elem; ++i)
 *                {
 *                    dtemp = records_(i); ar & dtemp;
 *                }
 *
 *                ar & samplingInterval_;
 *                ar & numberOfSamples_;
 *                ar & file_;
 *            }
 *            template<typename Archive>
 *            void load(Archive& ar, unsigned int version)
 *            {
 *                int i, rows, cols, itemp;
 *                double dtemp;
 *
 *                nodes_.reset();
 *                ar & rows;
 *                ar & cols;
 *                nodes_.set_size(rows, cols);
 *                for (i = 0; i < nodes_.n_elem; ++i)
 *                {
 *                     ar & dtemp; nodes_(i) = dtemp;
 *                }
 *                // lines
 *                lines_.reset();
 *                ar & rows;
 *                ar & cols;
 *                lines_.set_size(rows, cols);
 *                for (i = 0; i < lines_.n_elem; ++i)
 *                {
 *                    ar & itemp; lines_(i) = itemp;
 *                }
 *                // surfaces
 *                surfaces_.reset();
 *                ar & rows;
 *                ar & cols;
 *                for (i = 0; i < surfaces_.n_elem; ++i)
 *                {
 *                    ar & itemp; surfaces_(i) = itemp;
 *                }
 *                // records
 *                records_.reset();
 *                ar & rows;
 *                ar & cols;
 *                records_.set_size(rows, cols);
 *                for (i = 0; i < records_.n_elem; ++i)
 *                {
 *                    ar & dtemp; records_(i) = dtemp;
 *                }
 *
 *                ar & samplingInterval_;
 *                ar & numberOfSamples_;
 *                ar & file_;
 *            }
 */

//            BOOST_SERIALIZATION_SPLIT_MEMBER()

} // namespace Oi

#endif
