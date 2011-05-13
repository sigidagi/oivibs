// =====================================================================================
// 
//       Filename:  OiUniversalFileFormat.h
// 
//    Description:  Utilities functions
// 
//        Version:  1.0
//        Created:  2010-09-06 21::14
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

#ifndef _UNIVERSALFORMAT_H
#define _UNIVERSALFORMAT_H

#include    "formats/OiUFactory.h"
#include	"OiChannelInfo.h"
#include    "OiFileFormat.h"
#include    <vector>
#include    <string>
#include    <armadillo>
#include    <boost/shared_ptr.hpp>
#include	<boost/tuple/tuple.hpp>

using boost::shared_ptr;
using boost::tuple;
using std::vector;
using std::string;

namespace Oi {

    // forward declaration.
    class Root;

    class UniversalFileFormat : public FileFormatInterface
    {
    
    // pivate data
    private:

        Root* root_;
        string file_; 
    
        arma::Mat<double> nodes_;
        arma::Mat<unsigned int> lines_;
        arma::Mat<unsigned int> surfaces_;
        arma::Mat<double> channels_;
        
        vector<ChannelInfo> channelInfo_;

        // 1st tuple variable - uff format nummber, 2nd - position in file, 3rd - number of lines.
        vector< tuple<int, int, int> > info_;

        // holds raw data after parsing uff file.
        vector< shared_ptr<UFF> > uffObjects_;
        
        typedef vector< shared_ptr<UFF> >::iterator uffIterator;
        UFactory<int> uffFactory_;
        
    // private methods    
    private:     
        // helper function for creating matrix of records
        int getNumberOfSamples();
        void loadChannelInfo(uffIterator it, int nsteps);
        void loadChannels();
        
        template<typename T>
        void loadGeometry( arma::Mat<T>& geo, Category category);
        
    public:
        UniversalFileFormat(Root* owner, const string& file);
        ~UniversalFileFormat();

        // FileFormatInterface
        
        // First pass. It just find information: uff type or format, position in file 
        // and how big that format is (number of lines).
        // Second pass is accomplished by created UFF object.
        void parse();

        bool existNodes() const;
        bool existLines() const;
        bool existSurfaces() const;
        bool existChannels() const;
        int getNumberOfChannels() const;
        
        const arma::mat& getChannels() const;
        double getSamplingInterval() const;

        const double* getNodes(int& nrows, int& ncols) const;
        const unsigned int* getLines(int& nrows, int& ncols) const;
        const unsigned int* getSurfaces(int& nrows, int& ncols) const;
        
        // other public methods
        string getFileName() const;
        const vector<ChannelInfo>* getChannelInfo() const;
        /*
         *void save();
         *void load();
         */
    };

    
} // namespace Oi

#endif
