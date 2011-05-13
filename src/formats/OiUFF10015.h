// =====================================================================================
// 
//       Filename:  OiUFF10015.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-02-21 10:39:11
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


#ifndef  OIUFF10015_INC
#define  OIUFF10015_INC


#include	"OiUFF.h"
#include	<string>
#include	<vector>
#include	<algorithm>
#include	<fstream>
#include    <sstream>
#include	<iostream>

using std::vector;
using std::string;
using std::stringstream;


namespace Oi {

class UFF10015 : public UFF
{
    private:
        vector<double> nodes_;
        string file_;
        int position_;
        int numberOfLines_;

    public:
        UFF10015() : file_(""), position_(0), numberOfLines_(0) {}
        UFF10015(const string& file, int pos, int nlines) : file_(file), position_(pos), numberOfLines_(nlines) {} 
        
        typedef UFF10015 uff_type;

        const int number() const { return 10015; }
        Category category() const { return NODES; }
        
        void setParameters(const string& file, int position, int nlines)
        {
            file_ = file;
            position_ = position;
            numberOfLines_ = nlines;
        }

        const void* getData(int& nrows, int& ncols)
        {
            nrows = nodes_.size()/3;
            ncols = 3;
            return reinterpret_cast<void*>(&nodes_[0]);
        }

        void parse()
        {
            if (file_.empty() || numberOfLines_ == 0)
                return;
            
            // if already contain data - thereis no need to do it again.
            if (!nodes_.empty())
                return;

            std::ifstream fileStream;
            fileStream.open(file_.c_str(), std::ios::in);
            if (!fileStream.is_open())
            {
                std::cerr << "UFF10015::parse --\n";
                std::cerr << "Can NOT open file: " << file_ << "\n";
                return;
            }
            
            fileStream.seekg(position_, std::ios::beg);
            
            vector< vector<double> > coll;
            int nodeNumber(0); 
            string line;
            stringstream ss;
        
            double x(0.0), y(0.0), z(0.0);
            
            vector<int> nodeNumberList;
            coll.resize(numberOfLines_);

            for (int j = 0; j < numberOfLines_; ++j)
            {
                getline(fileStream, line);
                ss.str("");
                ss.clear();
                ss << line;
                ss >> nodeNumber;
                
                nodeNumberList.push_back(nodeNumber);
                ss >> x >> y >> z;
                
                coll[j].push_back(x);
                coll[j].push_back(y);
                coll[j].push_back(z);
            }

            fileStream.close();

            // in some case nodes in a file can be presented not in ascending order.
            // following procedure rearange matrix that first node starts in first column,
            // second - in second column.
            vector<int>::const_iterator it;
            vector<int>::const_iterator it_first = nodeNumberList.begin();
            vector<int>::const_iterator it_next;
            vector<int>::const_iterator it_last = nodeNumberList.end();
            
            int number(1);
            // now fill up matrix (variable UniversalFileFormat::nodes__) 
            for (it_next = it_first; it_next != it_last; ++it_next)
            { 
                // node numbers starts from 1.
                it = std::find(it_first, it_last, number);
                if (it != it_last && it != it_next)
                {
                    std::swap(nodeNumberList[it-it_first], nodeNumberList[it_next-it_first]);
                    std::swap(coll[it-it_first], coll[it_next-it_first]); 
                    ++number;
                }
            }
            
            for (size_t i = 0; i < 3; ++i)
            {
                for (size_t j = 0; j < coll.size(); ++j)
                    nodes_.push_back( coll[j][i] );
            }

            /*
             *if(!std::equal( nodeNumberList.begin(), nodeNumberList.end(), numberList.begin()))
             *{
             *        std::cerr << "UFF::NodeInfo::parse --\n";
             *        std::cerr << "Inconsistency in Node numbering!\n";
             *}
             */

        } // end of parse()

};

} // namespace Oi

#endif   // ----- #ifndef OIUFF10015_INC  -----
