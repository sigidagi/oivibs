#include "OiStorage.h"
#include "OiAsciiFileFormat.h"
#include "OiUtil.h"
#include <sstream>
#include <unistd.h>


namespace Oi {

    AsciiFileFormat::AsciiFileFormat(StorageInterface* storage) 
            : FileFormatInterface(storage), existData_(false), existSamplingInterval_(false)
    {

    }

    void AsciiFileFormat::parse(const string& file)
    {

        string path = Oi::stripToPath(file);
        chdir(path.c_str());

        string fileName = Oi::stripToFileName(file);

        bool status = data_.load(file, arma::raw_ascii);            
        if (status == false)
        {
            if (searchSamplingInterval(fileName))
                existSamplingInterval_ = true;        
        }
        else
            existData_ = true;

    }

    bool AsciiFileFormat::searchSamplingInterval(const string& fileName)
    {

        fileStream_.open(fileName.c_str(), std::ios::in);
        if (fileStream_.is_open())
        { 
            // return stream to the beginning.
            fileStream_.seekg(0, std::ios::beg);

            string line;
            std::stringstream ss;
            double dT = 0.0;

            while (!fileStream_.eof())
            {
                getline(fileStream_, line);
                if (fileStream_.eof())
                    break;

                ss << line;
                if (line == "T\r" && !fileStream_.eof())
                {
                    getline(fileStream_, line);
                    ss.str("");
                    ss.clear();
                    ss << line;
                    ss >> dT;
                    samplingInterval_ = dT;
                     
                    break;
                }
                
            }

            fileStream_.close();
       
            if (dT == 0.0)
                return false;
        }
   
        return true;
    }

    bool AsciiFileFormat::existNodes()
    {
        return false;
    }

    bool AsciiFileFormat::existLines()
    {
        return false;
    }

    bool AsciiFileFormat::existSurfaces()
    {
        return false;
    }

    bool AsciiFileFormat::existData()
    {
        return existData_;
    }

    const arma::mat& AsciiFileFormat::getNodes()
    {
        return nodes_;
    }

    const arma::umat& AsciiFileFormat::getLines()
    {
        return lines_;
    }

    const arma::umat& AsciiFileFormat::getSurfaces()
    {
        return surfaces_;
    }

    const arma::mat& AsciiFileFormat::getData()
    {
        return data_;
    }

    double AsciiFileFormat::getSamplingInterval()
    {
        return samplingInterval_;
    }

    int AsciiFileFormat::getNumberOfSamples()
    {
        return numberOfSamples_;
    }

} // namespace Oi
