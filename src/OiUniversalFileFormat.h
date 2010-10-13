#ifndef _UNIVERSALFORMAT_H
#define _UNIVERSALFORMAT_H

#include "OiFileFormat.h"
#include <vector>
#include <fstream>
#include <string>
#include <armadillo>
#include <boost/shared_ptr.hpp>

using std::pair;
using boost::shared_ptr;
using std::vector;
using std::string;
using std::streampos;

namespace Oi {

    // forward declaration.
    class Root;

    class UniversalFileFormat : public FileFormatInterface
    {
        // pivate data
    private:
        class Info 
        {
            public:
                Info(FileFormatInterface* self, const string& file) : self_(self), file_(file) {}
                virtual void parse() = 0;
                void setPosition(int pos) { position_ = pos; }
            protected:
                FileFormatInterface* self_;
                string file_;
                int position_;
        };

        class NodeInfo : public Info
        {
            public:
                NodeInfo(FileFormatInterface* self, const string& file);
                void parse();
        };

        class LineInfo : public Info
        {
            public:
                LineInfo(FileFormatInterface* self, const string& file);
                void parse();
        };

        class SurfaceInfo : public Info
        {
            public:
                SurfaceInfo(FileFormatInterface* self, const string& file);
                void parse();
        };

        class RecordInfo: public Info
        {
            public:
                RecordInfo(FileFormatInterface* self, const string& file, int recordnumber);
                void parse();
                double getSamplingInterval();
                int getNumberOfSamples();
            private:
                int recordnumber_;
                double samplingInterval_;
                int numberOfSamples_;
        };

        vector< shared_ptr<Info> > info_;

    private:
        template<typename T>
        bool existInfo()
        {
            vector< shared_ptr<Info> >::iterator it;
            for (it = info_.begin(); it != info_.end(); ++it)
            {
                if(boost::dynamic_pointer_cast<T>(*it))
                    return true;
            }
            return false;
        }
        
    public:
        UniversalFileFormat(Root* owner);
        ~UniversalFileFormat();

        // FileFormatInterface
        void parse(const string& file);

        bool existNodes();
        bool existLines();
        bool existSurfaces();
        bool existRecords();

        arma::mat& getNodes();
        arma::umat& getLines();
        arma::umat& getSurfaces();
        arma::mat& getRecords();

        /*
         *void save();
         *void load();
         */
    };

    
} // namespace Oi

#endif
