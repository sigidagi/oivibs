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
#include	"formats/OiUFFAll.h"

#include	<iomanip>
#include    <algorithm>
#include	<iterator>
#include    <sstream>
#include    <unistd.h>
#include    <boost/bind.hpp>

using boost::get;

namespace Oi {

    
    UniversalFileFormat::UniversalFileFormat(Root* owner, const string& file) : root_(owner), file_(file)
    {
        // Universal Dataset number is ID for registration of supported Universal Formats.
        uffFactory_.registerClass<UFF15>(15, "nodes");
        uffFactory_.registerClass<UFF10015>(10015, "nodes");
        
        uffFactory_.registerClass<UFF82>(82, "lines");
        uffFactory_.registerClass<UFF10082>(10082, "lines");
        uffFactory_.registerClass<UFF82>(82, "lines");

        uffFactory_.registerClass<UFF2412>(2412, "surfaces");
        uffFactory_.registerClass<UFF10012>(10012, "surfaces");
        
        uffFactory_.registerClass<UFF58>(58, "records");
        uffFactory_.registerClass<UFF10058>(10058, "records");
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
        
        std::cout << "File: " << file_ << "\n";
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

        try {

            std::for_each(uffObjects_.begin(), uffObjects_.end(), boost::bind(&UFF::parse, _1));
        }
        catch (bad_lexical_cast& el)
        {
            std::cerr << el.what() << std::endl;
        }
        catch (bad_numeric_cast& en)
        {
            std::cout << en.what() << std::endl;
        }
        catch (string& es)
        {
            std::cerr << es << std::endl;
        }

        loadGeometry(nodes_, "nodes"); 
        loadGeometry(lines_, "lines"); 
        loadGeometry(surfaces_, "surfaces"); 
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
        
        // get first sampling interval and compare with others.
        double samplingInterval = channelInfo_[0].sampling();

        // ----- Check if sampling interval in all records is the same.
        size_t nvalues = std::count_if(channelInfo_.begin(),
                                       channelInfo_.end(),
                                       bind(std::equal_to<double>(),
                                       bind(bind(&ChannelInfo::sampling, _1),
                                       _1),
                                       samplingInterval));

        if (nvalues != channelInfo_.size())
        {
            std::cerr << "Sampling Intervals differs in records!\n";
        }
        
        // all sampling interval values are equal. Return first one.
        return samplingInterval;
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
        vector<int> unumbers;
        std::transform(uffObjects_.begin(),
                       uffObjects_.end(),
                       back_inserter(unumbers),
                       boost::bind(&UFF::number, _1));                   
      
        // ----------------------------- RECORDS-------------------------------
        //
        // acccess records as raw data and form arma matrix
        vector<int> recordKeys;
        uffFactory_.selectKeysByCategory( recordKeys, "records");

        // loop through possible dataset numbers. First match with existed uffObject
        // will be chosen that dataset number as number represented recorded data.
        if (recordKeys.empty())
            return;
       
        vector<int>::const_iterator cit;
        cit = std::find_first_of(unumbers.begin(), unumbers.end(), recordKeys.begin(), recordKeys.end()); 
        if (cit == unumbers.end())
            return;
        
        // count how many object carry data.
        int count = (int)std::count(unumbers.begin(), unumbers.end(), *cit); 

        // if we are here - we found existing records. variable "count" holds how many uffObject
        // exist, iterator iit position holds information where uff object as record is.
    
        uffIterator uit = uffObjects_.begin();
        int nsteps = (int)(cit - unumbers.begin());
        std::advance(uit, nsteps);
        
       
        int nrows(0), ncols(0); 
        // TODO: needs better implementation.
        if (count == 1)
        {
            // then is suposed that that one object can hold several channels
            //
            const double* praw = reinterpret_cast<const double*>((*uit)->getData(nrows, ncols));
            channels_.set_size(nrows, ncols);

            for (int i = 0; i < ncols; ++i)
            {
                double* pdata = channels_.colptr(i);
                std::copy(praw+(i*nrows), praw+((i+1)*nrows), pdata);
            }
        }
        else
        {
            this->loadChannelInfo(uit, count);
            int nsamples = this->getNumberOfSamples();
            channels_.set_size(nsamples, count);

            // Channel matrix (column number represent record number) is initialized.
            // Next step copy data to that matrix.
            for (int i = 0; i < count; ++i, ++uit)
            {
                double* pdata = channels_.colptr(i);
                const double* praw = reinterpret_cast<const double*>((*uit)->getData(nrows, ncols));
                if (praw == 0)
                    continue;

                std::copy(praw, praw+(nrows+ncols), pdata);
            }

        }
    } // end of function 
 
    template<typename T>
    void UniversalFileFormat::loadGeometry( arma::Mat<T>& geo, const string& category)
    {
        vector<int> keys;
        uffFactory_.selectKeysByCategory(keys, category);
        if (keys.empty())
            return;

        // existing universal dataset numbers.
        vector<int> unumbers;
        std::transform(uffObjects_.begin(),
                       uffObjects_.end(),
                       back_inserter(unumbers),
                       boost::bind(&UFF::number, _1));                   

        vector<int>::const_iterator cit;
        cit = std::find_first_of(unumbers.begin(), unumbers.end(), keys.begin(), keys.end());
        if (cit == unumbers.end())
            return;

        uffIterator uit = uffObjects_.begin();
        std::advance( uit, (int)(cit - unumbers.begin()) ); 

        int nrows(0), ncols(0);
        const T* praw = reinterpret_cast<const T*>((*uit)->getData(nrows, ncols));

        geo.set_size(nrows, ncols);
        T* pdata = geo.memptr();
        std::copy(praw, praw+(nrows*ncols), pdata);
     } 
     
     template void UniversalFileFormat::loadGeometry<double>( arma::Mat<double>& geo, const string& category);
     template void UniversalFileFormat::loadGeometry<unsigned int>( arma::Mat<unsigned int>& geo, const string& category);
    
    
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

    string UniversalFileFormat::getFileName() const
    {
        return file_;
    }

    const double* UniversalFileFormat::getNodes(int& nrows, int& ncols) const
    {
        nrows = nodes_.n_rows;
        ncols = nodes_.n_cols;
        return nodes_.memptr();
    }
 
    const unsigned int* UniversalFileFormat::getLines(int& nrows, int& ncols) const
    {
        nrows = lines_.n_rows;
        ncols = lines_.n_cols;
        return lines_.memptr();
    }
   
    const unsigned int* UniversalFileFormat::getSurfaces(int& nrows, int& ncols) const
    {
        nrows = surfaces_.n_rows;
        ncols = surfaces_.n_cols;
        return surfaces_.memptr();
    }

    const arma::mat& UniversalFileFormat::getChannels() const
    {
        return channels_;
    }
    
    const vector<ChannelInfo>* UniversalFileFormat::getChannelInfo() const
    {
        return &channelInfo_; 
    }

} // namespace Oi

