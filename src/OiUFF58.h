// =====================================================================================
// 
//       Filename:  OiUFF58.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-01-18 14:55:53
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


#ifndef  OIUFF58_INC
#define  OIUFF58_INC

#include	"OiUFF.h"
#include	<string>
#include	<vector>
#include	<algorithm>
#include	<fstream>
#include    <sstream>

using std::vector;
using std::string;
using std::stringstream;

namespace Oi {

class UFF58 : public UFF
{
    private:
        vector<double> records_;
        string file_;
        string name_;
        int position_;
        int numberOfLines_;
        int numberOfSamples_;
        double samplingInterval_;

    public:
        UFF58() : file_(""), position_(0), numberOfLines_(0) {}
        UFF58(const string& file, int pos, int nlines) : file_(file), position_(pos), numberOfLines_(nlines) {}
        
        typedef UFF58 uff_type;
        
        const int number() const { return 58; }

        void setParameters(const string& file, int position, int nlines)
        {
            file_ = file;
            position_ = position;
            numberOfLines_ = nlines;
        }

        const void* getData(size_t& size)
        {
            size = numberOfSamples_; 
            return reinterpret_cast<void*>(&records_[0]);
        }

        void getExtraData(std::list<boost::any>& coll)
        {
            coll.clear();
            coll.push_back(samplingInterval_);
            coll.push_back(name_);
        }
        
        void parse()
        {
            if ( file_.empty() || numberOfLines_ == 0)
                return;
            
            // if once it is parsed - data exist and there is no need to repeat.
            if (!records_.empty())
                return;
            
            // find out dimmensions of record - number of samples;
            // parse header
            std::ifstream fileStream;
            fileStream.open(file_.c_str(), std::ios::in);
            if (!fileStream.is_open())
                return;
            
            fileStream.seekg(position_, std::ios::beg);

            string line;
            stringstream ss;

            getline(fileStream, line);
            name_ = line;

            getline(fileStream, line);
            getline(fileStream, line);
            getline(fileStream, line);
            getline(fileStream, line);
            getline(fileStream, line);
            getline(fileStream, line);

            double temp(0.0);
            ss << line;
            ss >> temp >> numberOfSamples_ >> temp >> temp >> samplingInterval_;
            ss.str("");
            ss.clear();
            // end parse header
          
            // reserve memory for records_ data.
            records_.reserve(numberOfSamples_);
            double value(0.0);

            getline(fileStream, line);
            getline(fileStream, line);
            getline(fileStream, line);
            getline(fileStream, line);

            for (int j = 0; j < numberOfLines_; ++j)
            {
                getline(fileStream, line);
                ss << line;
                ss >> value;
                do 
                {
                    records_.push_back(value);
                }
                while (ss >> value);
            
                ss.str("");
                ss.clear();
            }
            
            fileStream.close();
        }

};

} // namespace Oi

#endif   // ----- #ifndef OIUFF58_INC  -----
