// =====================================================================================
// 
//       Filename:  OiUFF10058.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-02-21 14:16:04
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


#ifndef  OIUFF10058_INC
#define  OIUFF10058_INC


#include	"OiUFF.h"
#include	"OiChannelInfo.h"
#include	<string>
#include	<vector>
#include	<algorithm>
#include	<iterator>
#include    <sstream>

using std::vector;
using std::string;
using std::stringstream;

namespace Oi {

class UFF10058 : public UFF
{
    private:
    
        string file_;
        int position_;
        int numberOfLines_;
       
        vector<double> records_;
        int nrows_;
        int ncols_;
        ChannelInfo info_; 
        
    public:
        UFF10058() : file_(""), position_(0), numberOfLines_(0) {}
        UFF10058(const string& file, int pos, int nlines) : file_(file), position_(pos), numberOfLines_(nlines) {}
        
        typedef UFF10058 uff_type;
        
        const int number() const { return 10058; }
        Category category() const { return RECORDS; }

        void setParameters(const string& file, int position, int nlines)
        {
            file_ = file;
            position_ = position;
            numberOfLines_ = nlines;
        }

        const void* getData(int& nrows, int& ncols)
        {
            nrows = nrows_;
            ncols = ncols_;
            return reinterpret_cast<void*>(&records_[0]);
        }

        boost::any getExtraData()
        {
            boost::any toReturn = info_;
            return toReturn;
        }
        
        void parse()
        {
            if ( file_.empty() )                
            {
                throw string("UFF10058\n Nothing to parse, no file exist!");
            }
            
            stringstream ss;
            if (numberOfLines_ < 3)
            {
                ss << "UFF10058\n Format is too small. Number of lines: " << numberOfLines_; 
                throw ss.str(); 
            }

            // ---------------------------parse header------------------------
            std::ifstream fileStream;
            fileStream.open(file_.c_str(), std::ios::in);
            if (!fileStream.is_open())
            {
                ss << "UFF10058\n Can't open the file: " << file_;
                throw ss.str();
            }
            
            fileStream.seekg(position_, std::ios::beg);
            string line;
            
            getline(fileStream, line);
            // Record 1. sampling interval 
            getline(fileStream, line);


            string whitespaces(" \t\v\f");
            // trim white space from beginning of line.
            size_t pos = line.find_first_not_of(whitespaces);
            line = line.substr(pos == string::npos ? 0 : pos);

            info_.sampling( boost::lexical_cast<double>(line) );
            getline(fileStream, line);
            // --------------------------end parse header------------------------
          

            // --------------------------- parse data -------------------------------
            // reserve memory for records_ data.
            records_.clear();
            
            fileStream.read((char*)&nrows_, sizeof(int));  
            fileStream.read((char*)&ncols_, sizeof(int));  
                
            records_.clear();
            records_.resize(nrows_*ncols_);
            vector<float> buffer(nrows_);

            for (int i = 0; i < ncols_; ++i)
            {
                fileStream.read((char*)&buffer[0], nrows_*sizeof(float));
                std::copy(buffer.begin(), buffer.end(), records_.begin() + (i*nrows_)); 
            }

            fileStream.close();
        }

};

} // namespace Oi

#endif   // ----- #ifndef OIUFF10058_INC  -----
