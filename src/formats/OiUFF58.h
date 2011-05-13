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
#include	"OiChannelInfo.h"
#include	<string>
#include	<vector>
#include	<algorithm>
#include	<iterator>
#include    <sstream>
#include	<boost/lexical_cast.hpp>
#include	<boost/numeric/conversion/cast.hpp>

using std::vector;
using std::string;
using std::stringstream;
using boost::lexical_cast;
using boost::numeric_cast;
using boost::bad_lexical_cast;
using boost::bad_numeric_cast;

namespace Oi {



class UFF58 : public UFF
{
    private:
    
        string file_;
        int position_;
        int numberOfLines_;
       
        vector<double> records_;
        ChannelInfo info_; 
        
    public:
        UFF58() : file_(""), position_(0), numberOfLines_(0) {}
        UFF58(const string& file, int pos, int nlines) : file_(file), position_(pos), numberOfLines_(nlines) {}
        
        typedef UFF58 uff_type;
        
        const int number() const { return 58; }
        Category category() const { return RECORDS; }

        void setParameters(const string& file, int position, int nlines)
        {
            file_ = file;
            position_ = position;
            numberOfLines_ = nlines;
        }

        const void* getData(int& nrows, int& ncols)
        {
            nrows = records_.size(); 
            ncols = 1;
            return reinterpret_cast<void*>(&records_[0]);
        }

        boost::any getExtraData()
        {
            boost::any toReturn = info_;
            return toReturn;
        }
        
        void parse()
        {
            stringstream ss;
            
            if ( file_.empty() )                
            {
                throw string("UFF58\n Nothing to parse, no file exist!");
            }
            
            if (numberOfLines_ < 11)
            {
                ss << "UFF58\n Format is too small. Number of lines: " << numberOfLines_; 
                throw ss.str(); 
            }

            // ---------------------------parse header------------------------
            std::ifstream fileStream;
            fileStream.open(file_.c_str(), std::ios::in);
            if (!fileStream.is_open())
            {
                ss << "UFF58\n Can't open the file: " << file_;
                throw ss.str();
            }
            
            fileStream.seekg(position_, std::ios::beg);

            string line;
            unsigned int nfields(0);
            
            // Record 1. ID or name. 
            getline(fileStream, line);
            info_.name( line );

            advanceLines(fileStream, 4);

            // Record 6. filed 6: response node and field 7: response direction. 
            getline(fileStream, line);
            ss << line;
            std::istream_iterator<string> string_it;
            string_it = ss;

            nfields = std::distance(string_it, std::istream_iterator<string>()); 
            if (nfields != 10)
            {
                ss.clear(); ss.str("");
                ss << "UFF58\n";
                ss << "Record 6 should hold 10 fields, counted only: " << nfields;
                throw ss.str();
            }

            // clear error flag and return to beginning.
            ss.clear(); ss.seekg(0, std::ios::beg);

            std::advance(string_it, 6);
            // field 6 - node.            
            info_.node( numeric_cast<unsigned int>(lexical_cast<int>(*string_it)) );  
            std::advance(string_it, 1);
            // field 7 - direction
            info_.direction( boost::lexical_cast<int>(*string_it) );

            ss.str(""); ss.clear(); ss.seekg(0, std::ios::beg);
            
            // Record 7. field 2: number of samples and field 5: sampling interval.
            getline(fileStream, line);
            ss << line;
            string_it = ss; 
            
            nfields = std::distance(string_it, std::istream_iterator<string>());
            if (nfields != 6)
            {
                ss.clear(); ss.str("");
                ss << "UFF58\n";
                ss << "Record 7 should hold 6 fields, counted only: " << nfields;
                throw ss.str();
            }

            ss.clear(); ss.seekg(0, std::ios::beg);
            
            std::advance(string_it, 2);
            info_.nsamples( numeric_cast<unsigned int>(lexical_cast<int>(*string_it)) );
            std::advance(string_it, 3);
            info_.sampling( boost::lexical_cast<double>(*string_it) );

            ss.str(""); ss.clear();
            // --------------------------end parse header------------------------
          
            advanceLines(fileStream, 4);
           


            // --------------------------- parse data -------------------------------
            // reserve memory for records_ data.
            records_.clear();
            records_.reserve(info_.nsamples());
            double value(0.0);
            
            // 11 lines takes header of UFF58
            for (int j = 0; j < numberOfLines_ - 11; ++j)
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
            // ---------------------------- end of parse data --------------------------
            
            if (info_.nsamples() != static_cast<unsigned int>(records_.size()))
            {
                ss.clear(); ss.str("");
                ss << "UFF58 Warning!  ";
                ss << "Size mismatch between saved data(" << records_.size();
                ss << ") and recorded number in the header(" << info_.nsamples() << ")\n"; 
                std::cerr << ss.str();
//                throw ss.str();
            }

            fileStream.close();
        }

};

} // namespace Oi

#endif   // ----- #ifndef OIUFF58_INC  -----
