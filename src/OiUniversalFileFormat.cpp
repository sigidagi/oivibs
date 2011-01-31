// =====================================================================================
// 
//       Filename:  OiUniversalFileFormat.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-10 13:26:09
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

#include "OiUniversalFileFormat.h"
#include "OiUtil.h"
#include "OiRoot.h"

#include "OiUFF15.h"
#include "OiUFF58.h"
#include "OiUFF82.h"
#include "OiUFF2412.h"

#include <algorithm>
#include	<iterator>
#include <sstream>
#include <unistd.h>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH 

using boost::get;

namespace Oi {

    
    UniversalFileFormat::UniversalFileFormat(Root* owner, const string& file) : FileFormatInterface(owner, file)
    {
        // Universal Dataset number is ID for registration of supported Universal Formats.
        uffFactory_.registerClass<UFF15>(15, "nodes");
        uffFactory_.registerClass<UFF82>(82, "lines");
        uffFactory_.registerClass<UFF2412>(2412, "surfaces");
        uffFactory_.registerClass<UFF58>(58, "records");
    }

    UniversalFileFormat::~UniversalFileFormat()
    {

    }
   
    bool UniversalFileFormat::isTag(string& line)
    {
        string whitespaces(" \t\v\f\n\r");

        // trim white space from beginning of line.
        size_t pos = line.find_first_not_of(' ');
        line = line.substr(pos == string::npos ? 0 : pos);
        
        pos = line.find_last_not_of(whitespaces);
        if (pos != string::npos)
            line.erase(pos + 1);
        else
            line.clear();
        
        if (line == "-1")
            return true;
        
        return false;
    }

    // Pass 1
    void UniversalFileFormat::parse()
    {
        if (file_.empty())
        {
            std::cerr << "UniversalFileFormat::parse -- file: " << file_ << " is empty!\n";
            return;
        }
        
        std::ifstream fileStream;

        fileStream.open(file_.c_str(), std::ios::in);
        if (fileStream.is_open())
        {
            // serches for nodes, lines, surfaces and data in UFF file.

            fileStream.seekg(0, std::ios::beg);
            info_.clear(); 
            string line;
            int format(0), position(0), numberOfLines(0);

 
            while (!fileStream.eof())
            {
                getline(fileStream, line);
                if (isTag(line) && !fileStream.eof())
                {
                    getline(fileStream, line);
                    from_string(format, line, std::dec);
                    position = fileStream.tellg();
                    numberOfLines = 0;

                    while (!fileStream.eof())
                    {
                        getline(fileStream, line);
                        if (isTag(line))
                            break;

                        ++numberOfLines;
                    }
                    
                    info_.push_back(boost::make_tuple(format, position, numberOfLines));
                }
            }
                        
                            
            fileStream.close();

        } // if fileStream.is_open()
        
        // Check if found in a file UFF format is supported by the program.
        // And if supported - initialize UFF object and set relevant parameters for later parsing.
        for (size_t i = 0; i < info_.size(); ++i)
        {
            if (uffFactory_.hasClass( get<0>(info_[i]) ))
            {
                 // first parameter - UFF format type(integer). It'll be used as ID to create UFF object. 
                 shared_ptr<UFF> uff(uffFactory_.createObject( get<0>(info_[i]) ));
                 // second and third parameters: position in file and number of lines to parse accordingly.
                 uff->setParameters(file_, get<1>(info_[i]), get<2>(info_[i])); 


                 uffObjects_.push_back( uff );
            }     
            else
                std::cerr << "Found format: " << get<0>(info_[i]) << " is not supported!\n";
        }
        
        // One UFF object are found and initialized - parse those objects according their format. 
        // UFF object will be filed with data from parsed file. 
        
        std::for_each(uffObjects_.begin(), uffObjects_.end(), boost::bind(&UFF::parse, _1));
        

        loadGeometry(nodes_, "nodes", 3); 
        loadGeometry(lines_, "lines", 2); 
        loadGeometry(surfaces_, "surfaces", 3); 
        
        std::cout << "Test\n";
        loadRecords();

    } // method end
    
    // return pair as first - position in uffObject vector, second - number of such objects in a row.
    void UniversalFileFormat::loadRecords()
    {
        if (uffObjects_.empty())
            return ;

        // existing universal dataset numbers.
        vector<int> univ_numbers;
        std::transform(uffObjects_.begin(),
                       uffObjects_.end(),
                       back_inserter(univ_numbers),
                       boost::bind(&UFF::number, _1));                   
      
        
        // ----------------------------- RECORDS-------------------------------
        //
        // acccess records as raw data and form arma matrix
        vector<int> recordKeys;
        uffFactory_.selectKeysByCategory( recordKeys, "records");


        int count(0);
        size_t idx(0);
        vector<int>::const_iterator iit = univ_numbers.end();
        // 
        // loop through possible dataset numbers. First match with existed uffObject
        // will be chosen that dataset number as number represented recorded data.
        if (!recordKeys.empty())
        {
            for (idx = 0; idx < recordKeys.size(); ++idx)
            {
                count = (int)std::count(univ_numbers.begin(), univ_numbers.end(), recordKeys[idx]); 
                if (count != 0)
                    break;
            }

            if (count != 0)
            {
                // if we are here - we found existing records. variable "count" holds how many uffObject
                // exist, idx - as index.
                iit = std::search_n(univ_numbers.begin(), univ_numbers.end(), count, recordKeys[idx]);
            }
            
            if (count != 0 && iit != univ_numbers.end())
            {
                vector< shared_ptr<UFF> >::iterator uit = uffObjects_.begin();
                std::advance(uit, (int)(iit - univ_numbers.begin()));

                
                // first find minimum number of samples.
                int i(0);
                vector<int> nsamples;
                size_t sz(0);
    
                for (i = 0; i < count; ++i)
                {
                    (*uit)->getData(sz);
                    nsamples.push_back(sz);
                    ++uit;
                }
                for (i = 0; i < count; ++i, --uit) { ; }
                
                numberOfSamples_ = *std::min_element(nsamples.begin(), nsamples.end());
                records_.set_size(numberOfSamples_, count);

                for (int i = 0; i < count; ++i, ++uit)
                {
                    double* pdata = records_.colptr(i);
                    const void* praw = (*uit)->getData(sz);
                    if (praw == 0)
                        continue;

                    std::memcpy(pdata, praw, numberOfSamples_*sizeof(double));
                }
                
            }
               
        } // if (!recordKeys.empty())

    } // end of function 
    

/*
 *    void UniversalFileFormat::save()
 *    {
 *        if (existNodes())
 *        {
 *            std::stringstream ss;
 *            pushToStream(ss, file_, nodes_); 
 *
 *            root_->getStorage()->write(ss); 
 *        }
 *    }
 *
 *    void UniversalFileFormat::load()
 *    {
 *
 *    }
 */

    
    bool UniversalFileFormat::existNodes()
    {
        return !nodes_.is_empty();
    }

    bool UniversalFileFormat::existLines()
    {
        return !lines_.is_empty(); 
    }

    bool UniversalFileFormat::existSurfaces()
    {
        return !surfaces_.is_empty();
    }

    bool UniversalFileFormat::existRecords()
    {
        return !records_.is_empty();
    }

} // namespace Oi

