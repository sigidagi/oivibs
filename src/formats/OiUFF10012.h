// =====================================================================================
// 
//       Filename:  OiUFF10012.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-02-21 14:00:21
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


#ifndef  OIUFF10012_INC
#define  OIUFF10012_INC


#include	"OiUFF.h"
#include	<string>
#include	<vector>
#include	<algorithm>
#include	<fstream>
#include    <sstream>

using std::vector;
using std::string;

namespace Oi {

class UFF10012 : public UFF
{
    private:
        vector<int> surfaces_;
        string file_;
        int position_;
        int numberOfLines_;
    
    public:
        UFF10012() : file_(""), position_(0), numberOfLines_(0) {}
        UFF10012(const string& file, int pos, int nlines) : file_(file), position_(pos), numberOfLines_(nlines) {}
        
        typedef UFF10012 uff_type;

        const int number() const { return 10012; }
        Category category() const { return LINES; }

        void setParameters(const string& file, int position, int nlines)
        {
            file_ = file;
            position_ = position;
            numberOfLines_ = nlines;
        }

        const void* getData(int& nrows, int& ncols)
        {
            nrows = surfaces_.size()/3;
            ncols = 3; 
            return reinterpret_cast<void*>(&surfaces_[0]);
        }

        void parse()
        {
            if (file_.empty() || numberOfLines_ == 0)
                return;
            
            // already exist data, no need to parse again.
            if (!surfaces_.empty())
                return;

            std::ifstream fileStream;
            fileStream.open(file_.c_str(), std::ios::in);
            if (!fileStream.is_open())
            {
                std::cerr << "UFF10012::parse --\n";
                std::cerr << "Can NOT open file: " << file_ << "\n";
                return;
            }

            fileStream.seekg(position_, std::ios::beg);

            string line;
            std::stringstream ss;
            int node1(0), node2(0), node3(0);
            
            vector<int> vNode1;
            vector<int> vNode2;
            vector<int> vNode3;

            for (int j = 0; j < numberOfLines_ -1; ++j)
            {
                getline(fileStream, line);
                ss.str("");
                ss.clear();
                ss << line;

                ss >> node1 >>  node2 >> node3; 
                
                vNode1.push_back(node1);
                vNode2.push_back(node2);
                vNode3.push_back(node3);
            }

            fileStream.close();
            surfaces_.clear();
            
            std::copy(vNode1.begin(), vNode1.end(), back_inserter(surfaces_));
            std::copy(vNode2.begin(), vNode2.end(), back_inserter(surfaces_));
            std::copy(vNode3.begin(), vNode3.end(), back_inserter(surfaces_));
        }
};

} // namespace Oi

#endif   // ----- #ifndef OIUFF10012_INC  -----
