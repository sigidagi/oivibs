// =====================================================================================
// 
//       Filename:  OiFileFormatInterface.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-09 10:31:18
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

#include    "OiRoot.h"
#include    "OiUniversalFileFormat.h"
#include    "OiFileFormat.h"
#include	"OiAsciiFileFormat.h"
#include    "OiUtil.h"

namespace Oi 
{
    FileFormatInterface::FileFormatInterface(Root* owner, const string& file) : root_(owner)
    {
        string path = Oi::stripToPath(file);
        chdir(path.c_str());

        // save file name, 
        file_ = Oi::stripToFileName(file);
    }

    shared_ptr<FileFormatInterface> FileFormatInterface::createFileFormat(Root* owner, const string& file)
    {
            string extension = Oi::stripToExtension(file);

            if (extension == "uff")
            {
                return shared_ptr<FileFormatInterface>(new UniversalFileFormat(owner, file));
            }
            else if (extension == "txt")
            {
                return shared_ptr<FileFormatInterface>(new AsciiFileFormat(owner, file));
            }
            else
            {
                shared_ptr<FileFormatInterface> pt;
                return pt;
            }
    }

    double FileFormatInterface::getSamplingInterval() const
    {
        return samplingInterval_;
    }

    int FileFormatInterface::getNumberOfSamples() const
    {
        return numberOfSamples_;
    }

    string FileFormatInterface::getFileName()
    {
        return file_;
    }

/*
 *    void FileFormatInterface::save()
 *    {
 *
 *        if (existNodes())
 *        {
 *            string variableName = string("nodes");
 *            SerializableObject<arma::mat> nodes = SerializableObject<arma::mat>(file_, variableName, nodes_);
 *            root_->getStorage()->write(nodes);                        
 *        }
 *        if (existLines())
 *        {
 *            string variableName = string("lines");
 *            SerializableObject<arma::umat> lines= SerializableObject<arma::umat>(file_, variableName, lines_);
 *            root_->getStorage()->write(lines);
 *        }
 *        if (existSurfaces())
 *        {
 *            string variableName = string("surfaces");
 *            SerializableObject<arma::umat> surfaces = SerializableObject<arma::umat>(file_, variableName, surfaces_);
 *            root_->getStorage()->write(surfaces);
 *        }
 *    }
 *
 *    void FileFormatInterface::load()
 *    {
 *
 *    }
 */

    const arma::mat& FileFormatInterface::getNodes() const 
    {
        return nodes_;
    }

    const arma::umat& FileFormatInterface::getLines() const 
    {
        return lines_;
    }

    const arma::umat& FileFormatInterface::getSurfaces() const
    {
        return surfaces_;
    }

    const arma::mat& FileFormatInterface::getRecords() const
    {
        return records_;
    }
  
} // namspace Oi
