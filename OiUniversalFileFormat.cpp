#include "OiUniversalFileFormat.h"
#include "OiUtil.h"
#include "OiStorage.h"

#include <algorithm>
#include <sstream>
#include <unistd.h>
#include <boost/bind.hpp>

using namespace std;

namespace Oi {

    typedef UniversalFileFormat UFF;
    
    UniversalFileFormat::UniversalFileFormat(StorageInterface* storage) : FileFormatInterface(storage)
    {
        
    }

    UniversalFileFormat::~UniversalFileFormat()
    {

    }

    void UniversalFileFormat::parse(const string& file)
    {
        if (file.empty())
            return;

        string path = Oi::stripToPath(file);
        chdir(path.c_str());

        string fileName = Oi::stripToFileName(file);
    
        std::ifstream fileStream;
        vector< shared_ptr<UFF::RecordInfo> > rInfo;
        int recordnumber = 0; 

        fileStream.open(fileName.c_str(), ios::in);
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
                    shared_ptr<UFF::NodeInfo> nodeinfo(new UFF::NodeInfo(this, file));
                    nodeinfo->setPosition(fileStream.tellg());
                    info_.push_back(nodeinfo);
                }
                // LineInfos
                else if (line == "82")
                {
                    shared_ptr<UFF::LineInfo> lineinfo(new UFF::LineInfo(this, file));
                    lineinfo->setPosition(fileStream.tellg());
                    info_.push_back(lineinfo);

                }
                // Surfaces
                else if (line == "2412")
                {
                    shared_ptr<UFF::SurfaceInfo> surfaceinfo(new UFF::SurfaceInfo(this, file));
                    surfaceinfo->setPosition(fileStream.tellg());
                    info_.push_back(surfaceinfo);
                }
                // Info
                else if (line == "58")
                {
                    recordnumber = (int)rInfo.size() + 1; 
                    shared_ptr<UFF::RecordInfo> recordinfo(new UFF::RecordInfo(this, file, recordnumber));
                    
                    recordinfo->setPosition(fileStream.tellg());
                    info_.push_back(recordinfo);
                    rInfo.push_back(recordinfo);
                }
            }

            fileStream.close();
        }

        if (info_.empty())
            return;

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
                // ERROR log will be implemented later. 
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

            // initiliaze matrix storage for later use.
            records_.set_size(numberOfSamples_, recordnumber); 
               
        }

        // parse every information (UFF::Info) found in a file and save into the storage class  
        std::for_each(info_.begin(), info_.end(), boost::bind(&UFF::Info::parse, _1)); 

                
    }

    UniversalFileFormat::NodeInfo::NodeInfo(FileFormatInterface* self, const string& file) : Info(self, file)
    {

    }

    void UniversalFileFormat::NodeInfo::parse()
    {
        std::ifstream fileStream;
        fileStream.open(file_.c_str(), ios::in);
        if (!fileStream.is_open())
            return;

        self_->getNodes().reset();
        fileStream.seekg(position_, ios::beg);

        int nodeNumber, temp; 
        string line;
        stringstream ss;
        double x, y, z;     

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
        
            ss >> temp >> temp >> temp;
            ss >> x >> y >> z;
           
            // reshape preserves elements in a matrix. 
            self_->getNodes().reshape(4, count);
            self_->getNodes()(0, count-1) = nodeNumber;
            self_->getNodes()(1, count-1) = x;
            self_->getNodes()(2, count-1) = y;
            self_->getNodes()(3, count-1) = z;
            
            ++count;
        }

        fileStream.close();
        
        // save nodes into the storage. it could be database or local file.
        self_->getStorage()->saveNodes(self_->getNodes());

    }

    UniversalFileFormat::LineInfo::LineInfo(FileFormatInterface* self, const string& file) : Info(self, file)
    {

    }

    void UniversalFileFormat::LineInfo::parse()
    {
        std::ifstream fileStream;
        fileStream.open(file_.c_str(), ios::in);
        if (!fileStream.is_open())
            return;

        self_->getLines().reset();    
        fileStream.seekg(position_, ios::beg);

        int temp, nNumberOfEntries, nTraceLineInfoNumber, nColor;
        string line;
        stringstream ss;
        vector<int> nodeList;

        getline(fileStream, line);
        ss << line;
        ss >> nTraceLineInfoNumber >> nNumberOfEntries >> nColor;

        if (nNumberOfEntries == 0)
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

        if ((int)nodeList.size() != nNumberOfEntries)
            return;

        vector<int>::iterator it;
        it = std::remove(nodeList.begin(), nodeList.end(), 0);
        if (it == nodeList.end())
            return;

        nodeList.erase(it, nodeList.end());
    
        self_->getLines().set_size(3, (int)(nodeList.size()/2)); 
        for (size_t i = 0; i < nodeList.size(); i+=2)
        {
            self_->getLines()(0, i/2) = (i/2+1);
            self_->getLines()(1, i/2) = nodeList[i];
            self_->getLines()(2, i/2) = nodeList[i+1];
        }
       
        fileStream.close();

        // save lines into the storage. it could be database or local file.
        self_->getStorage()->saveLines(self_->getLines());


        //catch ( std::out_of_range outOfRange )
        //{
        //	std::cout << "\n\nExeption: " << outOfRange.what();
        //	return;
        //}

        //existLineInfos_ = false;

    }
    
    UniversalFileFormat::SurfaceInfo::SurfaceInfo(FileFormatInterface* self, const string& file) : Info(self, file)
    {

    }

    void UniversalFileFormat::SurfaceInfo::parse()
    {
        std::ifstream fileStream;
        fileStream.open(file_.c_str(), ios::in);
        if (!fileStream.is_open())
            return;

        self_->getSurfaces().reset();   
        fileStream.seekg(position_, ios::beg);

        string line;
        stringstream ss;
        int SurfaceNumber, FEDescriptor, PhysicalPropNumber, MaterialPropNumber, Color, NumberOfElementNodeInfos;

        int count = 1;
        while (!fileStream.eof())
        {
            getline(fileStream, line);
            ss << line;
            ss >> SurfaceNumber;
            if (SurfaceNumber == -1)
                break;

            ss >> FEDescriptor >> PhysicalPropNumber >> MaterialPropNumber >> Color >> NumberOfElementNodeInfos;
            
            if ( FEDescriptor != 91 ) // Only thin shell triangular elements allowed
                return;

            if ( NumberOfElementNodeInfos != 3 ) // There need to be three nodes
                return;

            getline(fileStream, line);
            ss.str("");
            ss.clear();
            ss << line;

            int node1, node2, node3;
            ss >> node1 >>  node2 >> node3; 
            
            self_->getSurfaces().reshape(4, count);
            self_->getSurfaces()(0, count-1) = count;
            self_->getSurfaces()(1, count-1) = node1;
            self_->getSurfaces()(2, count-1) = node2;
            self_->getSurfaces()(3, count-1) = node3;
           
            ++count;
        }

        fileStream.close();

        // save lines into the storage. it could be database or local file.
        self_->getStorage()->saveSurfaces(self_->getSurfaces());


    }
    
    
    UniversalFileFormat::RecordInfo::RecordInfo(FileFormatInterface* self, const string& file, int recordnumber) : Info(self, file)
    {

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
        double value;

        getline(fileStream, line);
        getline(fileStream, line);
        getline(fileStream, line);
        getline(fileStream, line);
        getline(fileStream, line);
        getline(fileStream, line);
        getline(fileStream, line);

        int type;
        double temp;
        ss << line;
        ss >> type >> numberOfSamples_ >> temp >> temp >> samplingInterval_;
        ss.str("");
        ss.clear();

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

        if ( (int)self_->getRecords().n_cols == recordnumber_)
        {
            // save lines into the storage. it could be database or local file.
            self_->getStorage()->saveSurfaces(self_->getSurfaces());
        }

    }
  
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

