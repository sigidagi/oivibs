// =====================================================================================
// 
//       Filename:  OiUFF.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-01-18 12:57:59
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


#ifndef  OIUFF_INC
#define  OIUFF_INC

#include    <string>
#include	<fstream>
#include	<algorithm>
#include	<boost/any.hpp>

using std::string;

namespace Oi {

enum Category
{
    NODES, LINES, SURFACES, RECORDS 
};


class UFF
{
    public:
        typedef UFF uff_type;

        virtual const int number() const = 0;
        virtual void setParameters(const string& file, int position, int nlines) = 0;
        virtual void parse() = 0; 
        virtual Category category() const = 0;

        virtual const void* getData(int& nrows, int& ncols) = 0;
        virtual boost::any getExtraData() 
        {
            return boost::any();
        }

    protected:
        void advanceLines(std::ifstream& fs, unsigned int numberOfLines)
        {
            string temp;
            for (unsigned int i = 0; i < numberOfLines; ++i)
                getline(fs, temp);
        }
};

} // namespace Oi

#endif   // ----- #ifndef OIUFF_INC  -----
