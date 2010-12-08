#include "OiUniversalFileFormat.h"
#include "OiStorage.h"
#include "OiUtil.h"
#include "OiRoot.h"

#include <algorithm>
#include <sstream>
#include <unistd.h>
#include <boost/bind.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH 

using namespace std;
using boost::tuple;
using boost::make_tuple;

namespace Oi {

    typedef UniversalFileFormat UFF;
    
    UniversalFileFormat::UniversalFileFormat(Root* owner) : FileFormatInterface(owner)
    {
        
    }

    UniversalFileFormat::~UniversalFileFormat()
    {

    }

    void UniversalFileFormat::parse(const string& file)
    {
        if (file.empty())
        {
            cerr << "UniversalFileFormat::parse -- file: " << file << " is empty!\n";
            return;
        }

        string path = Oi::stripToPath(file);
        chdir(path.c_str());

        // save file name, later it will be used for persistency. 
        file_ = Oi::stripToFileName(file);
    
        std::ifstream fileStream;
        int recordnumber = 0; 

        fileStream.open(file_.c_str(), ios::in);
        if (fileStream.is_open())
        {
            // serches for nodes, lines, surfaces and data in UFF file.

            fileStream.seekg(0, ios::beg);
            info_.clear(); 
            string line;

            while (!fileStream.eof())
            {
                getline(fileStream, line);
                if (fileStream.eof())
                    break;

                // trim white space. 
                size_t pos1 =  line.find_first_not_of(' ');
                line = line.substr(pos1 == string::npos ? 0 : pos1);
                // remove curriage return (CR) if txt file is created in MSDOS.
                pos1 = line.find('\r');
                if (pos1 != string::npos)
                    line = line.substr(0, pos1);

                // if NodeInfos information 
                if (line == "15")
                {
                    shared_ptr<UFF::NodeInfo> nodeinfo(new UFF::NodeInfo(this, file_));
                    nodeinfo->setPosition(fileStream.tellg());
                    info_.push_back(nodeinfo);
                }
                // LineInfos
                else if (line == "82")
                {
                    shared_ptr<UFF::LineInfo> lineinfo(new UFF::LineInfo(this, file_));
                    lineinfo->setPosition(fileStream.tellg());
                    info_.push_back(lineinfo);

                }
                // Surfaces
                else if (line == "2412")
                {
                    shared_ptr<UFF::SurfaceInfo> surfaceinfo(new UFF::SurfaceInfo(this, file_));
                    surfaceinfo->setPosition(fileStream.tellg());
                    info_.push_back(surfaceinfo);
                }
                // Info
                else if (line == "58")
                {
                    shared_ptr<UFF::RecordInfo> recordinfo(new UFF::RecordInfo(this, file_, recordnumber));
                    ++recordnumber;
                    
                    recordinfo->setPosition(fileStream.tellg());
                    info_.push_back(recordinfo);
                }
            }

            fileStream.close();
        }

        if (info_.empty())
        {
            cerr << "UniversalFileFormat::parse --\n";
            cerr << "No information found in file: " << file << "\n";
            return;
        }

        vector< shared_ptr<UFF::RecordInfo> > rInfo;
        foreach( shared_ptr<UFF::Info> pinfo, info_)
        {
            if (boost::dynamic_pointer_cast<UFF::RecordInfo>(pinfo))
            {
                rInfo.push_back(boost::dynamic_pointer_cast<UFF::RecordInfo>(pinfo));
            }
        }

        std::for_each(rInfo.begin(), rInfo.end(), boost::bind(&UFF::RecordInfo::parseHeader, _1));

        if (!rInfo.empty())
        {
            // check if sampling interval is the same for every record.
            vector<double> samplingT;
            samplingT.resize(rInfo.size());

            std::transform(rInfo.begin(), 
                           rInfo.end(), 
                           samplingT.begin(), 
                           boost::bind(&UFF::RecordInfo::getSamplingInterval, _1));

            // if 
            if (!equal_elements(samplingT.begin(), samplingT.end()))
            {
                cerr << "UniversalFileFormat::parse --\n";
                cerr << "Sampling intervals differ in data records!\n";
                // one solution for such error would be delete those records ...... 
                return;
            }
           
            // as all element in container are equal - just take first one.
            samplingInterval_ = samplingT[0];
            
            // create vector which will contain number of samples for each record.
            vector<int> recordLenght;
            recordLenght.resize(rInfo.size());
           
            // loops through RecordInfo container and extract values. These values are saved in another container. 
            std::transform(rInfo.begin(), 
                           rInfo.end(), 
                           recordLenght.begin(), 
                           boost::bind(&UFF::RecordInfo::getNumberOfSamples, _1));

            
            // only minimal value of record length (number of samples) will be 
            // used in every record.
            numberOfSamples_ = *std::min_element(recordLenght.begin(), recordLenght.end());

            records_.set_size(numberOfSamples_, recordnumber); 
               
        }

        // parse every information (UFF::Info) found in a file and save into the storage class  
        std::for_each(info_.begin(), info_.end(), boost::bind(&UFF::Info::parse, _1)); 
      
        // save parsed elements: nodes, lines, records to the storage.
        this->save();
                
    }

    UniversalFileFormat::NodeInfo::NodeInfo(FileFormatInterface* self, const string& file) : Info(self, file)
    {

    }

    void UniversalFileFormat::NodeInfo::parse()
    {
        std::ifstream fileStream;
        fileStream.open(file_.c_str(), ios::in);
        if (!fileStream.is_open())
        {
            cerr << "UniversalFileFormat::NodeInfo::parse --\n";
            cerr << "Can NOT open file: " << file_ << "\n";
            return;
        }
        self_->getNodes().reset();
        fileStream.seekg(position_, ios::beg);

        int nodeNumber(0), temp(0); 
        string line;
        stringstream ss;
    
        double x(0.0), y(0.0), z(0.0);
        vector<int> nodeNumberList;
        arma::mat nodes;

        int count = 1;
        while (!fileStream.eof())
        {
            getline(fileStream, line);
            ss.str("");
            ss.clear();
            ss << line;
            ss >> nodeNumber;
            if (nodeNumber == -1)
                break;
        
            nodeNumberList.push_back(nodeNumber);
            ss >> temp >> temp >> temp;
            ss >> x >> y >> z;
        
            // reshape preserves elements in a matrix. 
            // NOTE: only if matrix is extended columnwise! 
            nodes.reshape(3, count);
            nodes(0, count-1) = x;
            nodes(1, count-1) = y;
            nodes(2, count-1) = z;
            
            ++count;
        }

        fileStream.close();
        
        // in some case nodes in a file can be presented not in ascending order.
        // following procedure rearange matrix that first node starts in first column,
        // second - in second column.
        vector<int>::const_iterator it;
        vector<int>::const_iterator beg = nodeNumberList.begin();
        self_->getNodes().reshape(nodeNumberList.size(), 3);
        // now fill up matrix (variable UniversalFileFormat::nodes__) 
        for (size_t i = 0; i < nodeNumberList.size(); ++i)
        {
            // node numbers starts from 1.
            
            it = std::find(nodeNumberList.begin(), nodeNumberList.end(), i+1);
            if (it == nodeNumberList.end())
            {
                cerr << "UFF::NodeInfo::parse --\n";
                cerr << "Inconsistency in Node numbering!\n";
            }
            
            //std::cout << (int)(it - nodeNumberList.begin()) << std::endl;

            self_->getNodes()(i,0) = nodes(0, it-beg);
            self_->getNodes()(i,1) = nodes(1, it-beg);
            self_->getNodes()(i,2) = nodes(2, it-beg);
        }

    }

    UniversalFileFormat::LineInfo::LineInfo(FileFormatInterface* self, const string& file) : Info(self, file)
    {

    }

    void UniversalFileFormat::LineInfo::parse()
    {
        std::ifstream fileStream;
        
        fileStream.open(file_.c_str(), ios::in);
        if (!fileStream.is_open())
        {
            cerr << "UniversalFileFormat::LineInfo::parse --\n";
            cerr << "Can NOT open file: " << file_ << "\n";  
            return;
        }

        self_->getLines().reset();    
        fileStream.seekg(position_, ios::beg);

        int temp(0), entries(0), traceLineInfo(0), color(0);
        string line;
        stringstream ss;
        vector<int> nodeList;

        getline(fileStream, line);
        ss << line;
        ss >> traceLineInfo >> entries >> color;

        // entries holds number of lines
        if (entries == 0)
            return;

        while(!fileStream.eof())
        {
            getline(fileStream, line);
            ss.str("");
            ss.clear();
            ss << line;

            while (ss >> temp)
            {
                if (temp == -1)
                    break;

                nodeList.push_back(temp);		
            }
            if (temp == -1)
                break;
        }

        fileStream.close();
        
        if ((int)nodeList.size() != entries)
            return;

        vector<int>::iterator it;
        it = std::remove(nodeList.begin(), nodeList.end(), 0);
        if (it == nodeList.end())
            return;

        nodeList.erase(it, nodeList.end());
    
        self_->getLines().set_size((int)(nodeList.size()/2), 2); 
        for (size_t i = 0; i < nodeList.size(); i+=2)
        {
            self_->getLines()(i/2, 0) = nodeList[i];
            self_->getLines()(i/2, 1) = nodeList[i+1];
        }
        
    }
    
    UniversalFileFormat::SurfaceInfo::SurfaceInfo(FileFormatInterface* self, const string& file) : Info(self, file)
    {

    }

    void UniversalFileFormat::SurfaceInfo::parse()
    {
        std::ifstream fileStream;
        fileStream.open(file_.c_str(), ios::in);
        if (!fileStream.is_open())
        {
            cerr << "UniversalFileFormat::SurfaceInfo::parse --\n";
            cerr << "Can NOT open file: " << file_ << "\n";
            return;
        }

        self_->getSurfaces().reset();   
        fileStream.seekg(position_, ios::beg);

        string line;
        stringstream ss;
        int surface(0), descriptor(0), physicalProperty(0),materialProperty(0), color(0), nodes(0);
        int node1(0), node2(0), node3(0);
        
        int count = 1;
        while (!fileStream.eof())
        {
            getline(fileStream, line);
            ss << line;
            ss >> surface;
            if (surface == -1)
                break;

            ss >> descriptor >> physicalProperty >> materialProperty >> color >> nodes;
            
            if ( descriptor != 91 ) // 91 - code of triangular elements 
                return;

            if ( nodes != 3 ) 
                return;

            getline(fileStream, line);
            ss.str("");
            ss.clear();
            ss << line;

            ss >> node1 >>  node2 >> node3; 
            
            self_->getSurfaces().reshape(3, count);
            self_->getSurfaces()(0, count-1) = node1;
            self_->getSurfaces()(1, count-1) = node2;
            self_->getSurfaces()(2, count-1) = node3;
           
            ++count;
        }

        fileStream.close();
        
        self_->getSurfaces() = arma::trans(self_->getSurfaces());

    }
    
    
    UniversalFileFormat::RecordInfo::RecordInfo(FileFormatInterface* self, const string& file, int recordnumber) : Info(self, file)
    {
        recordnumber_ =  recordnumber;
    }

    void UniversalFileFormat::RecordInfo::parseHeader()
    {
        std::ifstream fileStream;
        fileStream.open(file_.c_str(), ios::in);
        if (!fileStream.is_open())
            return;

        fileStream.seekg(position_, ios::beg);

        string line;
        stringstream ss;

        getline(fileStream, line);
        getline(fileStream, line);
        getline(fileStream, line);
        getline(fileStream, line);
        getline(fileStream, line);
        getline(fileStream, line);
        getline(fileStream, line);

        double temp(0.0);
        ss << line;
        ss >> temp >> numberOfSamples_ >> temp >> temp >> samplingInterval_;
        ss.str("");
        ss.clear();

        fileStream.close();
    }

    void UniversalFileFormat::RecordInfo::parse()
    {
        std::ifstream fileStream;
        fileStream.open(file_.c_str(), ios::in);
        if (!fileStream.is_open())
            return;

        fileStream.seekg(position_, ios::beg);

        string line;
        stringstream ss;
        double value(0.0);

        getline(fileStream, line);
        getline(fileStream, line);
        getline(fileStream, line);
        getline(fileStream, line);
        getline(fileStream, line);
        getline(fileStream, line);
        getline(fileStream, line);
        getline(fileStream, line);
        getline(fileStream, line);
        getline(fileStream, line);
        getline(fileStream, line);
        getline(fileStream, line);

        int count = 0;
        while (!fileStream.eof())
        {
            getline(fileStream, line);
            ss << line;
            ss >> value;
            if (value == -1.0 || count == numberOfSamples_ )
                break;
            
            do 
            {
                self_->getRecords()(count, recordnumber_) = value;
                ++count;
            }
            while (ss >> value);
        
            ss.str("");
            ss.clear();
        }

        fileStream.close();
    }
 
/*
 *    void UniversalFileFormat::save()
 *    {
 *        if (existNodes())
 *        {
 *            std::stringstream ss;
 *            pushToStream(ss, file_, nodes_); 
 *
 *            root_->getStorage()->write(ss); 
 *        }
 *    }
 *
 *    void UniversalFileFormat::load()
 *    {
 *
 *    }
 */

    double UniversalFileFormat::RecordInfo::getSamplingInterval()
    {
        return samplingInterval_;
    }

    int UniversalFileFormat::RecordInfo::getNumberOfSamples()
    {
        return numberOfSamples_;
    }
    
    arma::mat& UniversalFileFormat::getNodes()
    {
        return nodes_;
    }

    arma::umat& UniversalFileFormat::getLines()
    {
        return lines_;
    }

    arma::umat& UniversalFileFormat::getSurfaces()
    {
        return surfaces_;
    }

    arma::mat& UniversalFileFormat::getRecords()
    {
        return records_;
    }

    bool UniversalFileFormat::existNodes()
    {
        return existInfo<UFF::NodeInfo>(); 
    }

    bool UniversalFileFormat::existLines()
    {
        return existInfo<UFF::LineInfo>(); 
    }

    bool UniversalFileFormat::existSurfaces()
    {
        return existInfo<UFF::SurfaceInfo>(); 
    }

    bool UniversalFileFormat::existRecords()
    {
        return existInfo<UFF::RecordInfo>(); 
    }

} // namespace Oi

