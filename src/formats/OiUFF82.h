// =====================================================================================
// 
//       Filename:  OiUFF82.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-01-18 13:05:17
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


#ifndef  OIUFF82_INC
#define  OIUFF82_INC

#include	"OiUFF.h"
#include	<string>
#include	<vector>
#include	<fstream>
#include    <sstream>

using std::vector;
using std::string;

namespace Oi { 

class UFF82 : public UFF
{
    private:
        vector<int> lines_;
        string file_;
        int position_;
        int numberOfLines_;

    public:
        UFF82() : file_(""), position_(0), numberOfLines_(0) {}
        UFF82(const string& file, int pos, int nlines) : file_(file), position_(pos), numberOfLines_(nlines) {}
        
        typedef UFF82 uff_type;

        const int number() const { return 82; }
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

            int point(0), nlines(0), traceLineInfo(0), color(0);
            string line;
            std::stringstream ss;

            getline(fileStream, line);
            ss << line;
            ss >> traceLineInfo >> nlines >> color;

            // entries holds number of lines
            if (nlines == 0)
                return;
 
            vector<int> vtemp;
            vtemp.reserve(nlines);
           
            for (int j = 0; j < numberOfLines_-1; ++j)
            {
                getline(fileStream, line);
                ss.str("");
                ss.clear();
                ss << line;

                while (ss >> point)
                {
                    vtemp.push_back(point);		
                }
            }

            fileStream.close();
            
            if ((int)vtemp.size() != nlines)
                return;
    
            vector<int>::iterator it;
            it = std::remove(vtemp.begin(), vtemp.end(), 0);
            if (it == vtemp.end())
                return;

            vtemp.erase(it, vtemp.end());
            lines_.clear();
            
            size_t i(0);
            for (i = 0; i < vtemp.size(); i+=2)
                lines_.push_back( vtemp[i] );
 
            for (i = 1; i < vtemp.size(); i+=2)
                lines_.push_back( vtemp[i] );
           
        }
};

} // namespace Oi

#endif   // ----- #ifndef OIUFF82_INC  -----
