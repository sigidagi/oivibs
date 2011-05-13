// =====================================================================================
// 
//       Filename:  OiUFF2412.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-01-18 13:28:19
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


#ifndef  OIUFF2412_INC
#define  OIUFF2412_INC

#include	"OiUFF.h"
#include	<string>
#include	<vector>
#include	<algorithm>
#include	<fstream>
#include    <sstream>

using std::vector;
using std::string;

namespace Oi {

class UFF2412 : public UFF
{
    private:
        vector<int> surfaces_;
        string file_;
        int position_;
        int numberOfLines_;
    
    public:
        UFF2412() : file_(""), position_(0), numberOfLines_(0) {}
        UFF2412(const string& file, int pos, int nlines) : file_(file), position_(pos), numberOfLines_(nlines) {}
        
        typedef UFF2412 uff_type;

        const int number() const { return 2412; }
        Category category() const { return SURFACES; }

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
                std::cerr << "UFF2412::parse --\n";
                std::cerr << "Can NOT open file: " << file_ << "\n";
                return;
            }

            
            fileStream.seekg(position_, std::ios::beg);

            string line;
            std::stringstream ss;
            int surface(0), descriptor(0), physicalProperty(0),materialProperty(0), color(0), nnodes(0);
            int node1(0), node2(0), node3(0);
            
           
            // check only first line, if conditions is satisfied.
            getline(fileStream, line);
            ss << line;
            ss >> surface >> descriptor >> physicalProperty >> materialProperty >> color >> nnodes;
                
            if ( descriptor != 91 || nnodes != 3) // 91 - code of triangular elements, numberOfNodes - 3. 
                return;
            
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
            
            size_t i(0);
            surfaces_.clear();

            for (i = 0; i < vNode1.size(); i+=2)
                surfaces_.push_back( vNode1[i] );

            for (i = 0; i < vNode2.size(); i+=2)
                surfaces_.push_back( vNode2[i] );
            
            for (i = 0; i < vNode3.size(); i+=2)
                surfaces_.push_back( vNode3[i] );

        }
};

} // namespace Oi
#endif   // ----- #ifndef OIUFF2412_INC  -----
