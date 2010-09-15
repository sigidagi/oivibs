#include "OiUniversalFileFormat.h"
#include "OiUtil.h"

#include <algorithm>
#include <sstream>
#include <unistd.h>

using namespace std;

namespace Oi {

    UniversalFileFormat::UniversalFileFormat() : existNodes_(false), existLines_(false), existSurfaces_(false), existData_(false)
    {
        
    }

    void UniversalFileFormat::parse(const string& file)
    {
        if (file.empty())
            return;

        string path = Oi::stripToPath(file);
        chdir(path.c_str());

        string fileName = Oi::stripToFileName(file);

        fileStream_.open(fileName.c_str(), ios::in);
        if (fileStream_.is_open())
        {
            // serches for nodes, lines, surfaces and data in UFF file.
            searchForData();

            if (existNodes_)
            {
                parseNodes();
            }
            if (existLines_)
            {
                parseLines();
            }
            if (existSurfaces_)
            {
                parseSurfaces();
            }
            if (existData_)
            {
                for (size_t t = 0; t < dataPositionList_.size(); ++t)
                    parseData(dataPositionList_[t], t);
            }
            
            fileStream_.close();
        }

    }

    UniversalFileFormat::~UniversalFileFormat()
    {

    }

    void UniversalFileFormat::parseNodes()
    {
        if (!existNodes_)
            return;

        nodes_.reset();
        fileStream_.clear();
        fileStream_.seekg(nodePosition_, ios::beg);

        int nodeNumber, temp; 
        string line;
        stringstream ss;
        double x, y, z;     

        int count = 1;
        while (!fileStream_.eof())
        {
            getline(fileStream_, line);
            ss.str("");
            ss.clear();
            ss << line;
            ss >> nodeNumber;
            if (nodeNumber == -1)
                break;
        
            ss >> temp >> temp >> temp;
            ss >> x >> y >> z;
           
            // reshape preserves elements in a matrix. 
            nodes_.reshape(4, count);
            nodes_(0, count-1) = nodeNumber;
            nodes_(1, count-1) = x;
            nodes_(2, count-1) = y;
            nodes_(3, count-1) = z;
            
            ++count;
        }
        
    }

    void UniversalFileFormat::parseLines()
    {
        if (!existLines_)
            return;

        lines_.reset();    
        fileStream_.clear();
        fileStream_.seekg(linePosition_, ios::beg);

        int temp, nNumberOfEntries, nTraceLineNumber, nColor;
        string line;
        stringstream ss;
        vector<int> nodeList;

        getline(fileStream_, line);
        ss << line;
        ss >> nTraceLineNumber >> nNumberOfEntries >> nColor;

        if (nNumberOfEntries == 0)
            return;

        while(!fileStream_.eof())
        {
            getline(fileStream_, line);
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
    
        lines_.set_size(3, (int)(nodeList.size()/2)); 
        for (size_t i = 0; i < nodeList.size(); i+=2)
        {
            lines_(0, i/2) = (i/2+1);
            lines_(1, i/2) = nodeList[i];
            lines_(2, i/2) = nodeList[i+1];
        }
        
        //catch ( std::out_of_range outOfRange )
        //{
        //	std::cout << "\n\nExeption: " << outOfRange.what();
        //	return;
        //}

        //existLines_ = false;
    }

    void UniversalFileFormat::parseSurfaces()
    {
        if (!existSurfaces_)
            return;
        
        surfaces_.reset();   
        fileStream_.clear();
        fileStream_.seekg(surfacePosition_, ios::beg);

        string line;
        stringstream ss;
        int SurfaceNumber, FEDescriptor, PhysicalPropNumber, MaterialPropNumber, Color, NumberOfElementNodes;

        int count = 1;
        while (!fileStream_.eof())
        {
            getline(fileStream_, line);
            ss << line;
            ss >> SurfaceNumber;
            if (SurfaceNumber == -1)
                break;

            ss >> FEDescriptor >> PhysicalPropNumber >> MaterialPropNumber >> Color >> NumberOfElementNodes;
            
            if ( FEDescriptor != 91 ) // Only thin shell triangular elements allowed
                return;

            if ( NumberOfElementNodes != 3 ) // There need to be three nodes
                return;

            getline(fileStream_, line);
            ss.str("");
            ss.clear();
            ss << line;

            int node1, node2, node3;
            ss >> node1 >>  node2 >> node3; 
            
            surfaces_.reshape(4, count);
            surfaces_(0, count-1) = count;
            surfaces_(1, count-1) = node1;
            surfaces_(2, count-1) = node2;
            surfaces_(3, count-1) = node3;
           
            ++count;
        }
    }

    void UniversalFileFormat::parseData(const int pos, int column )
    {
        if (!existData_)
            return;

        fileStream_.clear();
        fileStream_.seekg(pos, ios::beg);

        string line;
        stringstream ss;
        double value;

        getline(fileStream_, line);
        getline(fileStream_, line);
        getline(fileStream_, line);
        getline(fileStream_, line);
        getline(fileStream_, line);
        getline(fileStream_, line);
        getline(fileStream_, line);

        int type, numberOfSamples;
        double samplingInterval, temp;
        ss << line;
        ss >> type >> numberOfSamples >> temp >> temp >> samplingInterval;
        ss.str("");
        ss.clear();


        if (samplingInterval_ == 0.0)
            samplingInterval_ = samplingInterval;
        else if (samplingInterval_ != samplingInterval)
        {
            // Report ERROR: sampling interval is differ for channels!
        }
        
        if (numberOfSamples_ == 0 || numberOfSamples < numberOfSamples_)
            numberOfSamples_ = numberOfSamples;

          
        // resize data channels without distroing values inside.   
        data_.set_size( numberOfSamples_, (int)dataPositionList_.size() );
        
        getline(fileStream_, line);
        getline(fileStream_, line);
        getline(fileStream_, line);
        getline(fileStream_, line);

        int count = 0;

        while (!fileStream_.eof())
        {
            getline(fileStream_, line);
            ss << line;
            ss >> value;
            if (value == -1.0 || count == numberOfSamples )
                break;

            do 
            {
                data_(count, column) = value;
                ++count;
            }
            while (ss >> value);
        
            ss.str("");
            ss.clear();
        }
    }


    void UniversalFileFormat::searchForData()
    {
        // return stream to the beginning.
        fileStream_.seekg(0, ios::beg);

        string line;

        while (!fileStream_.eof())
        {
            getline(fileStream_, line);
            if (fileStream_.eof())
                break;

            // trim white space. 
            size_t pos1 =  line.find_first_not_of(' ');
            line = line.substr(pos1 == string::npos ? 0 : pos1);
            // remove curriage return (CR) if txt file is created in MSDOS.
            pos1 = line.find('\r');
            if (pos1 != string::npos)
                line = line.substr(0, pos1);

            //std::cout << ".";
            // if Nodes information 
            if (line == "15")
            {
                //std::cout << "\n";
                //std::cout << "Nodes were found!\n";
                existNodes_ = true;
                nodePosition_ = fileStream_.tellg();
            }
            // Lines
            else if (line == "82")
            {
                //std::cout << "\n";
                //std::cout << "Lines were found!\n";
                existLines_ = true;
                linePosition_ = fileStream_.tellg();

            }
            // Surfaces
            else if (line == "2412")
            {
                //std::cout << "\n";
                //std::cout << "Surfaces were found!\n";
                existSurfaces_ = true;
                surfacePosition_ = fileStream_.tellg();
            }
            // Data
            else if (line == "58")
            {
                //std::cout << "\n";
                //std::cout << "Data was found!\n";
                existData_ = true;
                dataPositionList_.push_back(fileStream_.tellg());
            }
        }
        //std::cout << "\n\n";
    }

    const arma::mat& UniversalFileFormat::getNodes()
    {
        return nodes_;
    }

    const arma::umat& UniversalFileFormat::getLines()
    {
        return lines_;
    }

    const arma::umat& UniversalFileFormat::getSurfaces()
    {
        return surfaces_;
    }

    const arma::mat& UniversalFileFormat::getData()
    {
        return data_;
    }

    double UniversalFileFormat::getSamplingInterval()
    {
        return samplingInterval_;
    }

    int UniversalFileFormat::getNumberOfSamples()
    {
        return numberOfSamples_;
    }

    bool UniversalFileFormat::existNodes()
    {
        return existNodes_;
    }

    bool UniversalFileFormat::existLines()
    {
        return existLines_;
    }

    bool UniversalFileFormat::existSurfaces()
    {
        return existSurfaces_;
    }

    bool UniversalFileFormat::existData()
    {
        return existData_;
    }

} // namespace Oi

