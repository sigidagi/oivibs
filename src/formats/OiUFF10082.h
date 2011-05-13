// =====================================================================================
// 
//       Filename:  OiUFF10082.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-02-21 13:42:37
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


#ifndef  OIUFF10082_INC
#define  OIUFF10082_INC


#include	"OiUFF.h"
#include	<string>
#include	<vector>
#include	<fstream>
#include    <sstream>

using std::vector;
using std::string;

namespace Oi { 

class UFF10082 : public UFF
{
    private:
        vector<int> lines_;
        string file_;
        int position_;
        int numberOfLines_;

    public:
        UFF10082() : file_(""), position_(0), numberOfLines_(0) {}
        UFF10082(const string& file, int pos, int nlines) : file_(file), position_(pos), numberOfLines_(nlines) {}
        
        typedef UFF10082 uff_type;

        const int number() const { return 10082; }
        Category category() const { return LINES; }

        void setParameters(const string& file, int position, int nlines)
        {
            file_ = file;
            position_ = position;
            numberOfLines_ = nlines;
        }

        const void* getData(int& nrows, int& ncols)
        {
            nrows = lines_.size()/2;
            ncols = 2;
            return reinterpret_cast<void*>(&lines_[0]);
        }

        void parse()
        {
            if (file_.empty() || numberOfLines_ == 0)
                return;
            
            // if data exist there is no need to parse again
            if (!lines_.empty())
                return;

            std::ifstream fileStream;
            
            fileStream.open(file_.c_str(), std::ios::in);
            if (!fileStream.is_open())
            {
                std::cerr << "UniversalFileFormat::LineInfo::parse --\n";
                std::cerr << "Can NOT open file: " << file_ << "\n";  
                return;
            }
            
            fileStream.seekg(position_, std::ios::beg);

            string line;
            std::stringstream ss;
            
            int node1(0), node2(0);
            vector<int> vtemp1;
            vector<int> vtemp2;
            vtemp1.reserve(numberOfLines_);
            vtemp2.reserve(numberOfLines_);
           
            for (int j = 0; j < numberOfLines_-1; ++j)
            {
                getline(fileStream, line);
                ss.str("");
                ss.clear();
                ss << line;
                ss >> node1 >> node2;

                vtemp1.push_back(node1);		
                vtemp2.push_back(node2);		
            }
            
            fileStream.close();
            lines_.clear();
            
            std::copy(vtemp1.begin(), vtemp1.end(), back_inserter(lines_)); 
            std::copy(vtemp2.begin(), vtemp2.end(), back_inserter(lines_)); 
        }
};

} // namespace Oi

#endif   // ----- #ifndef OIUFF10082_INC  -----
