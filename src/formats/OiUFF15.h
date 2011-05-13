// =====================================================================================
// 
//       Filename:  OiUFF15.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-01-18 12:10:59
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


#ifndef  OIUFF15_INC
#define  OIUFF15_INC

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

class UFF15 : public UFF
{
    private:
        vector<double> nodes_;
        string file_;
        int position_;
        int numberOfLines_;

    public:
        UFF15() : file_(""), position_(0), numberOfLines_(0) {}
        UFF15(const string& file, int pos, int nlines) : file_(file), position_(pos), numberOfLines_(nlines) {} 
        
        typedef UFF15 uff_type;

        const int number() const { return 15; }
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
                std::cerr << "UFF15::parse --\n";
                std::cerr << "Can NOT open file: " << file_ << "\n";
                return;
            }
            
            fileStream.seekg(position_, std::ios::beg);

            int nodeNumber(0), temp(0); 
            string line;
            stringstream ss;
        
            double x(0.0), y(0.0), z(0.0);
            vector<int> nodeNumberList;
            vector<int> numberList;
            nodes_.resize(3*numberOfLines_);

            for (int j = 0; j < numberOfLines_; ++j)
            {
                getline(fileStream, line);
                ss.str("");
                ss.clear();
                ss << line;
                ss >> nodeNumber;
                
                numberList.push_back(1+j);
                nodeNumberList.push_back(nodeNumber);
                ss >> temp >> temp >> temp;
                ss >> x >> y >> z;
            
                nodes_[j] = x;
                nodes_[1*numberOfLines_+j] = y;
                nodes_[2*numberOfLines_+j] = z;
            }

            fileStream.close();
           
            // check if vector nodeNumberList contains values in ascending order.
            if(!std::equal( nodeNumberList.begin(), nodeNumberList.end(), numberList.begin()))
            {
                    std::cerr << "UFF::NodeInfo::parse --\n";
                    std::cerr << "Inconsistency in Node numbering!\n";
            }

/*
 *            // in some case nodes in a file can be presented not in ascending order.
 *            // following procedure rearange matrix that first node starts in first column,
 *            // second - in second column.
 *            vector<int>::const_iterator it;
 *            vector<int>::const_iterator beg = nodeNumberList.begin();
 *            nodes_.reshape(nodeNumberList.size(), 3);
 *            // now fill up matrix (variable UniversalFileFormat::nodes__) 
 *            for (size_t i = 0; i < nodeNumberList.size(); ++i)
 *            {
 *                // node numbers starts from 1.
 *                
 *                it = std::find(nodeNumberList.begin(), nodeNumberList.end(), i+1);
 *                if (it == nodeNumberList.end())
 *                {
 *                    std::cerr << "UFF::NodeInfo::parse --\n";
 *                    std::cerr << "Inconsistency in Node numbering!\n";
 *                }
 *                
 *                //std::cout << (int)(it - nodeNumberList.begin()) << std::endl;
 *
 *                nodes_(i,0) = nodes(0, it-beg);
 *                nodes_(i,1) = nodes(1, it-beg);
 *                nodes_(i,2) = nodes(2, it-beg);
 *            }
 */

        } // end of parse()

};

} // namespace Oi

#endif   // ----- #ifndef OIUFF15_INC  -----
