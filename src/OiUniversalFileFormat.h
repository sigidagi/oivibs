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

#include    "OiUFF.h"
#include    "OiFileFormat.h"
#include    <vector>
#include    <string>
#include    <armadillo>
#include    <boost/shared_ptr.hpp>
#include	<boost/tuple/tuple.hpp>

using boost::shared_ptr;
using boost::tuple;
using std::vector;
using std::string;

namespace Oi {

    // forward declaration.
    class Root;

    class UniversalFileFormat : public FileFormatInterface
    {
    
    private:
        // list of all possible universal dataset number which represent data (or records)

        // pivate data
    private:
        // 1st tuple variable - uff format nummber, 2nd - position in file, 3rd - number of lines.
        vector< tuple<int, int, int> > info_;
        vector< shared_ptr<UFF> > uffObjects_;
        
        UFactory<int> uffFactory_;
        
        typedef vector< shared_ptr<UFF> >::iterator uffIterator;
            
        bool isTag(string& line);

        void loadRecords();
        
        template<typename T>
        void loadGeometry( arma::Mat<T>& geo, const string& category, int ncols)
        {
            vector<int> keys;
            uffFactory_.selectKeysByCategory(keys, category);
        
            // existing universal dataset numbers.
            vector<int> univ_numbers;
            std::transform(uffObjects_.begin(),
                           uffObjects_.end(),
                           back_inserter(univ_numbers),
                           boost::bind(&UFF::number, _1));                   

            vector<int>::const_iterator iit = univ_numbers.end();
            if (!keys.empty())
            {
                iit = univ_numbers.end();
                for (size_t idx = 0; idx < keys.size(); ++idx)
                {
                    iit = std::find(univ_numbers.begin(), univ_numbers.end(), keys[idx]);
                    if ( iit != univ_numbers.end() )
                        break;
                }
                
                if ( iit != univ_numbers.end() )
                {
                    vector< shared_ptr<UFF> >::iterator uit = uffObjects_.begin();
                    std::advance( uit, (int)(iit - univ_numbers.begin()) ); 

                    size_t sz(0);
                    const void* praw = (*uit)->getData(sz);

                    geo.set_size(sz/ncols, ncols);
                    T* pdata = geo.memptr();
                    std::memcpy(pdata, praw, sz*sizeof(T));
                }
            }

        }

    public:
        UniversalFileFormat(Root* owner, const string& file);
        ~UniversalFileFormat();

        // FileFormatInterface
        void parse();

        bool existNodes();
        bool existLines();
        bool existSurfaces();
        bool existRecords();

        /*
         *void save();
         *void load();
         */
    };

    
} // namespace Oi

#endif
