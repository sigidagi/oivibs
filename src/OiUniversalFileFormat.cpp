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

#include    "OiUniversalFileFormat.h"
#include    "OiUtil.h"
#include    "OiRoot.h"

#include    "OiUFF15.h"
#include    "OiUFF58.h"
#include    "OiUFF82.h"
#include    "OiUFF2412.h"

#include	<iomanip>
#include    <algorithm>
#include	<iterator>
#include    <sstream>
#include    <unistd.h>
#include    <boost/bind.hpp>
#include    <boost/foreach.hpp>

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
            string whitespaces1(" \t\v\f");
            string whitespaces(" \t\v\f\n\r");
 
            while (!fileStream.eof())
            {
                getline(fileStream, line);

                // trim white space from beginning of line.
                size_t pos = line.find_first_not_of(whitespaces1);
                line = line.substr(pos == string::npos ? 0 : pos);
                
                pos = line.find_last_not_of(whitespaces);
                if (pos != string::npos)
                    line.erase(pos + 1);
                else
                    line.clear();

                if (line == "-1" && !fileStream.eof())
                {
                    getline(fileStream, line);
                    from_string(format, line, std::dec);
                    position = fileStream.tellg();
                    numberOfLines = 0;

                    while (!fileStream.eof())
                    {
                        getline(fileStream, line);
                        // trim white space from beginning of line.
                        pos = line.find_first_not_of(' ');
                        line = line.substr(pos == string::npos ? 0 : pos);
                        
                        pos = line.find_last_not_of(whitespaces);
                        if (pos != string::npos)
                            line.erase(pos + 1);
                        else
                            line.clear();

                        if (line == "-1")
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
                 std::cout << "Found universal dataset number: " << get<0>(info_[i]) << "\t";
                 std::cout << std::setw(10) << uffFactory_.selectCategory( get<0>(info_[i]) ) << "... OK\n";
                 // first parameter - UFF format type(integer). It'll be used as ID to create UFF object. 
                 shared_ptr<UFF> uff(uffFactory_.createObject( get<0>(info_[i]) ));
                 // second and third parameters: position in file and number of lines to parse accordingly.
                 uff->setParameters(file_, get<1>(info_[i]), get<2>(info_[i])); 


                 uffObjects_.push_back( uff );
            }     
            else
                std::cerr << "Found universal dataset number: " << get<0>(info_[i]) << " is not supported!\n";
        }
        
        // One UFF object are found and initialized - parse those objects according their format. 
        // UFF object will be filed with data from parsed file. 
        
        std::cout << std::endl;
        std::cout << "Parsing....\n";

        try {

            std::for_each(uffObjects_.begin(), uffObjects_.end(), boost::bind(&UFF::parse, _1));
        }
        catch (bad_lexical_cast& ecast)
        {
            std::cerr << ecast.what() << std::endl;
        }
        catch (bad_numeric_cast& en)
        {
            std::cout << en.what() << std::endl;
        }
        catch (string& estring)
        {
            std::cerr << estring << std::endl;
        }
        std::cout << "Done!\n";
        std::cout << std::endl;

        loadGeometry(nodes_, "nodes", 3); 
        loadGeometry(lines_, "lines", 2); 
        loadGeometry(surfaces_, "surfaces", 3); 
        loadChannels();

    } // method end
   
    void UniversalFileFormat::loadChannelInfo(uffIterator it, int nrecords)
    {
        boost::any anyInfo;
        ChannelInfo channelInfo; 

        for (int i = 0; i < nrecords; ++i, ++it)
        {
            anyInfo = (*it)->getExtraData();
            if (anyInfo.empty())
                continue;

            try { channelInfo = boost::any_cast<ChannelInfo>(anyInfo); }
            catch(boost::bad_any_cast& e) { std::cerr << e.what() << "\n"; continue; }

            channelInfo_.push_back(channelInfo);
        }
    }

    double UniversalFileFormat::getSamplingInterval() const
    {
        if (channelInfo_.empty())
            return -1;

        // ----- Check if sampling interval in all records is the same.
        vector<double> samplingIntervals;
        
        std::transform(channelInfo_.begin(),
                      channelInfo_.end(),
                      back_inserter(samplingIntervals),
                      boost::bind(&ChannelInfo::sampling, _1));

        size_t nvalues = std::count(samplingIntervals.begin(), samplingIntervals.end(), samplingIntervals[0]);

        if (nvalues != samplingIntervals.size())
        {
            std::cerr << "Sampling Intervals differs in records!\n";
        }
        
        // all sampling interval values are equal. Return first one.
        return samplingIntervals[0];
    }
    
    int UniversalFileFormat::getNumberOfSamples()
    {
        if (channelInfo_.empty())
            return 0;

        // find minimum number of samples. If there are difference - all records will be sliced
        // to such minimum number of samples.
        vector<int> nsamples;
        
        std::transform(channelInfo_.begin(),
                       channelInfo_.end(),
                       back_inserter(nsamples),
                       boost::bind(&ChannelInfo::nsamples, _1));
        
        return *std::min_element(nsamples.begin(), nsamples.end());
    }

    // return pair as first - position in uffObject vector, second - number of such objects in a row.
    void UniversalFileFormat::loadChannels()
    {
        if (uffObjects_.empty())
            return ;

        // First we create a vector of existing universal dataset numbers, and 
        // we have information (dataset number) of registered records. Loop through 
        // registred record numbers and find their existence in found UFF Objects. 
        // stop the loop when first match is found.

        // existing universal dataset numbers in a file.
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
                // exist, iterator iit position holds information where uff object as record is.
                iit = std::search_n(univ_numbers.begin(), univ_numbers.end(), count, recordKeys[idx]);
            }
            
            if (count != 0 && iit != univ_numbers.end())
            {
                uffIterator uit = uffObjects_.begin();
                int nsteps = (int)(iit - univ_numbers.begin());
                std::advance(uit, nsteps);
                
                loadChannelInfo(uit, count);
                int nsamples = getNumberOfSamples();
                
                channels_.set_size(nsamples, count);
                size_t sz(0); 

                // Channel matrix (column number represent record number) is initialized.
                // Next step copy data to that matrix.
                for (int i = 0; i < count; ++i, ++uit)
                {
                    double* pdata = channels_.colptr(i);
                    const void* praw = (*uit)->getData(sz);
                    if (praw == 0)
                        continue;

                    std::memcpy(pdata, praw, nsamples*sizeof(double));
                }
            }
        } // if (!recordKeys.empty())
    } // end of function 
 
    template<typename T>
    void UniversalFileFormat::loadGeometry( arma::Mat<T>& geo, const string& category, int ncols)
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
     
     template void UniversalFileFormat::loadGeometry<double>( arma::Mat<double>& geo, const string& category, int ncols);
     template void UniversalFileFormat::loadGeometry<unsigned int>( arma::Mat<unsigned int>& geo, const string& category, int ncols);
    
    
    bool UniversalFileFormat::existNodes() const
    {
        return !nodes_.is_empty();
    }

    bool UniversalFileFormat::existLines() const
    {
        return !lines_.is_empty(); 
    }

    bool UniversalFileFormat::existSurfaces() const
    {
        return !surfaces_.is_empty();
    }

    bool UniversalFileFormat::existChannels() const 
    {
        return !channels_.is_empty();
    }

} // namespace Oi

