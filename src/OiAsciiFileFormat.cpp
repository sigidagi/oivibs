// =====================================================================================
// 
//       Filename:  OiAsciiFileFormat.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-09 10:32:08
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

#include "OiRoot.h"
#include "OiAsciiFileFormat.h"
#include "OiUtil.h"
#include <sstream>

namespace Oi {

    AsciiFileFormat::AsciiFileFormat(Root* owner, const string& file) 
                : root_(owner), file_(file), existChannels_(false)
    {

    }

    void AsciiFileFormat::parse()
    {
        bool status = channels_.load(file_, arma::raw_ascii);            
        if (status == true)
            existChannels_ = true;
    }


    double AsciiFileFormat::getSamplingInterval() const
    {
        return 1.0;
    }

    bool AsciiFileFormat::existNodes() const
    {
        return false;
    }

    bool AsciiFileFormat::existLines() const  
    {
        return false;
    }

    bool AsciiFileFormat::existSurfaces() const
    {
        return false;
    }

    bool AsciiFileFormat::existChannels() const
    {
        return existChannels_;
    }
    
    const arma::mat& AsciiFileFormat::getChannels() const
    {
        return channels_;
    }
    
    string AsciiFileFormat::getFileName() const
    {
        return file_;
    }

    int AsciiFileFormat::getNumberOfChannels() const
    {
        return (int)(channels_.n_cols);
    }

/*
 *    bool AsciiFileFormat::searchSamplingInterval(const string& fileName)
 *    {
 *
 *        fileStream_.open(fileName.c_str(), std::ios::in);
 *        if (fileStream_.is_open())
 *        { 
 *            // return stream to the beginning.
 *            fileStream_.seekg(0, std::ios::beg);
 *
 *            string line;
 *            std::stringstream ss;
 *            double dT = 0.0;
 *
 *            while (!fileStream_.eof())
 *            {
 *                getline(fileStream_, line);
 *                if (fileStream_.eof())
 *                    break;
 *
 *                ss << line;
 *                if (line == "T\r" && !fileStream_.eof())
 *                {
 *                    getline(fileStream_, line);
 *                    ss.str("");
 *                    ss.clear();
 *                    ss << line;
 *                    ss >> dT;
 *                    samplingInterval_ = dT;
 *                     
 *                    break;
 *                }
 *                
 *            }
 *
 *            fileStream_.close();
 *       
 *            if (dT == 0.0)
 *                return false;
 *        }
 *   
 *        return true;
 *    }
 */
} // namespace Oi
